#include <QDebug>
#include <QVariant>
#include <iostream>
#include <utility>
#include "queryTemplates.h"
#include "sonicSocket.h"
#include "webManager.h"

WebSoloManager::WebSoloManager(std::vector<QString> a_words)
    : WebManager(std::move(a_words)) {
    inputter.setBufferMaxSize(dictionary.getCurrentWord().size() + 1);
}

void WebSoloManager::key_pressed(QChar button) {
    using client::queries::backspace_pressed_query;
    using client::queries::key_pressed_query;
    using client::web::socket;
    if (button == ' ') {
        if (inputter.getBuffer() == dictionary.getCurrentWord()) {
            inputter.addSymbol(button);
            socket().send(key_pressed_query(QString(button)));
            return;
        }
        if (!dictionary.nextWord()) {
            dictionary.previousWord();
            return;
        }
        dictionary.previousWord();

        inputter.addSymbol(button);
        dictionary.nextWord();
        inputter.setBufferMaxSize(dictionary.getCurrentWord().size() + 1);
        socket().send(key_pressed_query(QString(button)));
        emit print_signal(inputter.getBuffer(), 0);
        return;
    }
    if (inputter.addSymbol(button)) {
        socket().send(key_pressed_query(QString(button)));
        emit print_signal(inputter.getBuffer(), 0);
    }
}

void WebSoloManager::backspace_pressed() {
    using client::queries::backspace_pressed_query;
    using client::web::socket;
    if (inputter.getBuffer().size() == 0) {
        if (inputter.getBuffersCnt() == 1) {
            return;
        }
        dictionary.previousWord();
    }
    inputter.deleteSymbol();
    socket().send(backspace_pressed_query());
    emit print_signal(inputter.getBuffer(), 0);
}

QString WebSoloManager::get_buffer() {
    return inputter.getBuffer();
}

QString WebSoloManager::blob() {
    return dictionary.blob();
}

QVariant WebSoloManager::next() {
    return QVariant();
}
