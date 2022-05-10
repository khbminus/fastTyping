#include "webManager.h"
#include "queryTemplates.h"
#include "sonicSocket.h"
#include <QDebug>

WebManager::WebManager(std::vector<QString> a_words) : dictionary(a_words) {
}

void WebManager::key_pressed(QChar button) {
    using client::queries::buffer_clear_query;
    using client::queries::key_pressed_query;
    using client::queries::new_word_query;

    using client::web::socket;


    if (button == ' ' && clear_buffer) {
        clear_buffer = false;
        inputter.clearBuffer();
        //socket().send(buffer_clear_query());
        socket().send(new_word_query());
    } else {
        inputter.addSymbol(button);
        socket().send(key_pressed_query(QString(button)));
    }

    emit print_signal(inputter.getBuffer());
}

void WebManager::backspace_pressed() {
    using client::queries::backspace_pressed_query;
    using client::web::socket;

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
    clear_buffer = true;
    emit correct_signal();
}
