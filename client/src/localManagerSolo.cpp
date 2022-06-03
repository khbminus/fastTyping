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

// bool LocalManagerSolo::is_correct_word() {
//    return inputter.getBuffer() == dictionary.getCurrentWord();
//}

//////
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
            //            if (check_symbol(inputter.getBuffer().size() - 1)) {
            emit correct_signal();
            //            } else {
            //                emit error_signal();
            //            }
            return;
        } else {
            inputter.addSymbol(button);

            emit errorOnPositionSignal(dictionary.getCompletedSize() +
                                       inputter.getBuffer().size() - 1);
            if (!dictionary.nextWord()) {
                inputter.clearBuffer();
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
    emit correct_signal();

    //    if (check_symbol(inputter.getBuffer().size() - 1)) {
    //        emit correct_signal();
    //    } else {
    //        emit error_signal();
    //    }
    //    emit_correctness();
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
    //    if (!check_prefix()) {
    //        return {};
    //    }

    QString buffer = inputter.getBuffer();
    QString sample = dictionary.getCurrentWord();

    if (buffer.size() == sample.size()) {
        return QChar(' ');
    } else {
        return sample[buffer.size()];
    }
}
