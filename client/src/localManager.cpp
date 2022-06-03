#include "localManager.h"
#include <QDebug>
#include <QVariant>
#include <iterator>

#include <utility>

bool LocalManager::check_prefix() {
    QString buffer = inputter.getBuffer();
    QString sample = dictionary.getCurrentWord();
    qDebug() << "comparing" << buffer << " with " << sample;

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

bool LocalManager::check_symbol(int position) {
    QString buffer = inputter.getBuffer();
    QString sample = dictionary.getCurrentWord();
    if (sample.size() <= position)
        return false;
    return buffer[position] == sample[position];
}

LocalManager::LocalManager(std::vector<QString> a_words)
    : dictionary(std::move(a_words)) {
    inputter.setBufferMaxSize(dictionary.getCurrentWord().size() + 1);
}

bool LocalManager::is_correct_word() {
    return inputter.getBuffer() == dictionary.getCurrentWord();
}

void LocalManager::key_pressed(QChar button) {
    if (dictionary.isEnded()) {
        return;
    }
    if (button == ' ') {
        if (inputter.getBuffer() == dictionary.getCurrentWord()) {
            inputter.clearBuffer();
            wpmChartModel->backspace();
            if (!dictionary.nextWord()) {
                emit end_signal();
                return;
            }
            inputter.setBufferMaxSize(dictionary.getCurrentWord().size() + 1);
            emit print_signal(
                inputter.getBuffer(),
                dictionary.getCompletedSize() + inputter.getBuffer().size());
            emit_correctness();
            return;
        } else {
            inputter.addSymbol(button);
        }
    } else {
        inputter.addSymbol(button);
    }

    if (!check_symbol(inputter.getBuffer().size() - 1)) {
        wpmChartModel->errorSymbol(
            inputter.getBuffer().back(),
            dictionary.getCurrentWord()[inputter.getBuffer().size() - 1]);
        emit errorOnPositionSignal(dictionary.getCompletedSize() +
                                   inputter.getBuffer().size() - 1);
    } else {
        wpmChartModel->correctSymbol();
        emit correctOnPositionSignal(dictionary.getCompletedSize() +
                                     inputter.getBuffer().size() - 1);
    }
    emit print_signal(inputter.getBuffer(), dictionary.getCompletedSize() +
                                                inputter.getBuffer().size());
    emit_correctness();
}

void LocalManager::backspace_pressed() {
    inputter.deleteSymbol();
    emit correctOnPositionSignal(dictionary.getCompletedSize() +
                                 inputter.getBuffer().size());
    emit print_signal(inputter.getBuffer(), dictionary.getCompletedSize() +
                                                inputter.getBuffer().size());
    emit_correctness();
}

QString LocalManager::get_buffer() {
    return inputter.getBuffer();
}

QString LocalManager::blob() {
    return dictionary.blob();
}

QVariant LocalManager::next() {
    if (!check_prefix()) {
        return {};
    }

    QString buffer = inputter.getBuffer();
    QString sample = dictionary.getCurrentWord();

    if (buffer.size() == sample.size()) {
        return QChar(' ');
    } else {
        return sample[buffer.size()];
    }
}
