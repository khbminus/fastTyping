#include <QDebug>
#include <QVariant>
#include <iterator>
#include <utility>
#include "localManager.h"

// bool LocalManagerSolo::check_prefix() {
//    QString buffer = inputter.getBuffer();
//    QString sample = dictionary.getCurrentWord();
//    qDebug() << "comparing" << buffer << " with " << sample;
//
//    if (buffer.size() > sample.size()) {
//        return false;
//    }
//
//    return std::equal(buffer.begin(), buffer.end(), sample.begin());
//}

// void LocalManagerSolo::emit_correctness() {
//    if (check_prefix()) {
//        emit correct_signal();
//    } else {
//        emit error_signal();
//    }
//}

bool LocalManagerSolo::check_symbol(int position) {
    QString buffer = inputter.getBuffer();
    QString sample = dictionary.getCurrentWord();
    if (sample.size() <= position)
        return false;
    return buffer[position] == sample[position];
}

LocalManagerSolo::LocalManagerSolo(std::vector<QString> a_words)
    : LocalManager(std::move(a_words)) {
    inputter.setBufferMaxSize(dictionary.getCurrentWord().size() + 1);
}

void LocalManagerSolo::key_pressed(QChar button) {
    if (button == ' ') {
        wpmChartModel->backspace();
        if (inputter.getBuffer() == dictionary.getCurrentWord()) {
            inputter.clearBuffer();
            if (!dictionary.nextWord()) {
                emit end_signal();
                return;
            }
            inputter.setBufferMaxSize(dictionary.getCurrentWord().size() + 1);
            emit print_signal(
                inputter.getBuffer(),
                dictionary.getCompletedSize() + inputter.getBuffer().size());
            emit correct_signal();
            return;
        } else {
            if (!dictionary.nextWord()) {
                dictionary.previousWord();
                return;
            }
            dictionary.previousWord();
            inputter.addSymbol(button);
            emit errorOnPositionSignal(dictionary.getCompletedSize() +
                                       inputter.getBuffer().size() - 1);
            if (!dictionary.nextWord()) {
                emit end_signal();
                return;
            }
            inputter.setBufferMaxSize(dictionary.getCurrentWord().size() + 1);
            emit print_signal(
                inputter.getBuffer(),
                dictionary.getCompletedSize() + inputter.getBuffer().size());
            emit correct_signal();
            return;
        }
    } else {
        inputter.addSymbol(button);
    }
    if (!check_symbol(inputter.getBuffer().size() - 1)) {
        QVariant symbolFromDictionary;
        const auto &word = dictionary.getCurrentWord();
        if (inputter.getBuffer().size() <= word.size()) {
            symbolFromDictionary = word[inputter.getBuffer().size() - 1];
        }
        wpmChartModel->errorSymbol(inputter.getBuffer().back(),
                                   symbolFromDictionary);
        emit errorOnPositionSignal(dictionary.getCompletedSize() +
                                   inputter.getBuffer().size() - 1);
    } else {
        wpmChartModel->correctSymbol();
        emit correctOnPositionSignal(dictionary.getCompletedSize() +
                                     inputter.getBuffer().size() - 1);
    }

    emit print_signal(inputter.getBuffer(), dictionary.getCompletedSize() +
                                                inputter.getBuffer().size());
    emit correct_signal();
}

void LocalManagerSolo::backspace_pressed() {
    if (inputter.getBuffer().size() == 0) {
        if (inputter.getBuffersCnt() == 1) {
            return;
        }
        dictionary.previousWord();
    }

    inputter.deleteSymbol();
    emit correctOnPositionSignal(dictionary.getCompletedSize() +
                                 inputter.getBuffer().size());
    emit print_signal(inputter.getBuffer(), dictionary.getCompletedSize() +
                                                inputter.getBuffer().size());
    emit correct_signal();
}

QString LocalManagerSolo::get_buffer() {
    return inputter.getBuffer();
}

QString LocalManagerSolo::blob() {
    return dictionary.blob();
}

QVariant LocalManagerSolo::next() {
    QString buffer = inputter.getBuffer();
    QString sample = dictionary.getCurrentWord();

    if (buffer.size() >= sample.size()) {
        return QChar(' ');
    } else {
        return sample[buffer.size()];
    }
}
