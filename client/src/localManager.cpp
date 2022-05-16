#include "localManager.h"
#include <QDebug>
#include <iterator>
#include <utility>

bool LocalManager::check_prefix() {
    QString buffer = inputter.getBuffer();
    QString sample = dictionary.getCurrentWord();
    qDebug() << "copmaparing" << buffer << " with " << sample;

    if (buffer.size() > sample.size()) {
        return false;
    }

    return std::equal(buffer.begin(), buffer.end(), sample.begin());
}

void LocalManager::emit_correctness() {
    if (check_prefix()) {
        emit correct_signal();
    } else {
        emit error_signal();
    }
}

LocalManager::LocalManager(std::vector<QString> a_words)
    : dictionary(std::move(a_words)) {}

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
    emit print_signal(inputter.getBuffer());
    emit_correctness();
}

void LocalManager::backspace_pressed() {
    inputter.deleteSymbol();
    emit print_signal(inputter.getBuffer());
    emit_correctness();
}

QString LocalManager::get_buffer() {
    return inputter.getBuffer();
}

QString LocalManager::blob() {
    return dictionary.blob();
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
