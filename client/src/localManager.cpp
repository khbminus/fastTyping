#include "localManager.h"

void LocalManager::check_prefix(QString a, QString b) {
    if (a.size() > b.size()) {
        emit error_signal(a);
        return;
    }
    for (int ind = 0; ind < a.size(); ind++) {
        if (a[ind] != b[ind]) {
            emit error_signal(a);
            return;
        }
    }
}

LocalManager::LocalManager(std::vector<QString> a_words) : dictionary(a_words) {
}

void LocalManager::key_pressed(QChar button) {
    if (button == ' ') {
        if (inputter.getBuffer() == dictionary.getCurrentWord()) {
            inputter.clearBuffer();
            if (!dictionary.nextWord()) {
                emit end_signal();
                return;
            }
        } else {
            inputter.addSymbol(button);
        }
    } else {
        inputter.addSymbol(button);
    }
    check_prefix(inputter.getBuffer(), dictionary.getCurrentWord());
}

void LocalManager::backspace_pressed() {
    inputter.deleteSymbol();
    check_prefix(inputter.getBuffer(), dictionary.getCurrentWord());
}


QString LocalManager::get_buffer() {
    return inputter.getBuffer();
}
std::optional<QChar> LocalManager::next() {
    return std::nullopt;
}

