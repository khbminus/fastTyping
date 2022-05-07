#include "localManager.h"
#include <QDebug>

bool LocalManager::check_prefix() {
    QString buffer = inputter.getBuffer();
    QString sample = dictionary.getCurrentWord();
    qDebug() << "copmaparing" << buffer << " with " << sample;
    if (buffer.size() > sample.size()) {
        return false;
    }
    for (std::size_t ind = 0; ind < buffer.size(); ind++) {
        if (buffer[ind] != sample[ind]) {
            return false;
        }
    }

    return true;
}

void LocalManager::emit_correctness(){
    if (check_prefix()) {
        emit correct_signal();
    } else {
        emit error_signal();
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
    emit_correctness();
}

void LocalManager::backspace_pressed() {
    inputter.deleteSymbol();
    emit_correctness();
}

QString LocalManager::get_buffer() {
    return inputter.getBuffer();
}
std::optional<QChar> LocalManager::next() {
    if (!check_prefix()) {
        return std::nullopt;
    }
    QString buffer = inputter.getBuffer();
    QString sample = dictionary.getCurrentWord();

    if (buffer.size() == sample.size()) {
        return ' ';
    } else {
        return sample[buffer.size()];
    }
}

