#include "webManager.h"
#include <QDebug>
#include <QVariant>
#include <utility>
#include "queryTemplates.h"
#include "sonicSocket.h"

WebManager::WebManager(std::vector<QString> a_words)
    : dictionary(std::move(a_words)) {
    inputter.setBufferMaxSize(dictionary.getCurrentWord().size() + 1);
}

void WebManager::key_pressed(QChar button) {
    using client::queries::key_pressed_query;
    using client::web::socket;
    if (inputter.addSymbol(button)) {
        socket().send(key_pressed_query(QString(button)));
        emit print_signal(inputter.getBuffer(), 0);
    }
    qDebug() << "current web buffer" << inputter.getBuffer();
}

void WebManager::backspace_pressed() {
    using client::queries::backspace_pressed_query;
    using client::web::socket;

    inputter.deleteSymbol();
    socket().send(backspace_pressed_query());
    emit print_signal(inputter.getBuffer(), 0);
}

QString WebManager::get_buffer() {
    return inputter.getBuffer();
}

QString WebManager::blob() {
    return dictionary.blob();
}

QVariant WebManager::next() {
    return QVariant();
}

void WebManager::end_slot() {
    emit end_signal();
}
void WebManager::error_slot() {
    emit error_signal();
}
void WebManager::correct_slot() {
    emit correct_signal();
}

void WebManager::correct_word_slot() {
    inputter.clearBuffer();
    dictionary.nextWord();
    inputter.setBufferMaxSize(dictionary.getCurrentWord().size() + 1);
    emit print_signal(inputter.getBuffer(), 0);
    emit correct_signal();
}
