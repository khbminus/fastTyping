#include "gameClient.h"
#include "common/include/json.hpp"
#include <QString>
#include <iostream>
#include <string>

using nlohmann::json;

const short ECHO_PORT = 30566;
const short PORT = 1337;


json keyPressedRequest(QString buffer) {
    json result;
    result["header"] = {{"type", "addNewChar"}};
    result["body"] = {{"char", buffer.toStdString()}};
    return result;
}

json greetingRequest(QString name) {
    json result;
    result["header"] = {{"type", "hello"}};
    result["body"] = {{"name", name.toStdString()}};
    return result;
}

const json createGameRequest = R"({"header":{"type":"createGame"},"body":{"dictionaryName": "const", "parserName": "simple", "autoJoin":true ,"words": ["This", "is", "sample", "don't", "judge", "me"]}})"_json;
const json joinGameRequest = R"({"header": { "type": "joinGame"},"body": {"id": 0}})"_json;
const json backspaceRequest = R"({"header": { "type": "backspace"},"body": {}})"_json;

QString beautify(json request) {
    return QString::fromStdString(request.dump() + "\n");
}

namespace game::client {

    GameClient::GameClient(QString name) : socket_wrapper("46.17.248.131", PORT, this) {
        socket_wrapper.sent(beautify(greetingRequest(name)));
        socket_wrapper.sent(beautify(createGameRequest));
    }

    void GameClient::keyPressed(QChar button) {
        local_buffer.addSymbol(button);
        socket_wrapper.sent(beautify(keyPressedRequest(QString(button))));
    }

    void GameClient::backspacePressed() {
        socket_wrapper.sent(beautify(backspaceRequest));
        local_buffer.deleteSymbol();
    }

    [[nodiscard]] bool GameClient::getErrorStatus() const {
        return !mIsCorrect;
    }
    [[nodiscard]] bool GameClient::isEnded() const {
        return mIsEnded;
    }
    [[nodiscard]] QString GameClient::getBuffer() const {
        return local_buffer.getBuffer();
    }
}// namespace game::client
