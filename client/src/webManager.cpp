#include "webManager.h"
#include "sonicSocket.h"
#include "queryTemplates.h"
#include <QDebug>

WebManager::WebManager(std::vector<QString> a_words) : dictionary(a_words) {
}

void WebManager::key_pressed(QChar button) {
    using client::web::socket;
    using client::queries::key_pressed_query;

    inputter.addSymbol(button);
    emit print_signal(inputter.getBuffer());
    socket().send(key_pressed_query(QString(button)));
}

void WebManager::backspace_pressed() {
    using client::web::socket;
    using client::queries::backspace_pressed_query;

    inputter.deleteSymbol();
    emit print_signal(inputter.getBuffer());
    socket().send(backspace_pressed_query());
}

QString WebManager::get_buffer() {
    return inputter.getBuffer();
}

QString WebManager::blob() {
    return dictionary.blob();
}

std::optional<QChar> WebManager::next() {
    return std::nullopt;
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
    emit print_signal("");
    emit correct_signal();
}
