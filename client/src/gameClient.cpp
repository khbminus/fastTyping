#include "gameClient.h"
#include "common/include/json.hpp"
#include <QString>

using nlohmann::json;

const short ECHO_PORT = 30566;
const short PORT = 1337;


json keyPressedRequest(QString buffer) {
    json result;
    result["header"] = {{"type", "checkInput"}};
    result["body"] = {{"word", buffer.toStdString()}};
    return result;
}

json startGameRequest(QString name) {
    json result;
    result["header"] = {{"type", "hello"}};
    result["body"] = {{"name", name.toStdString()}};
    return result;
}

const json joinRequest = R"({"header":{"type":"createGame"},"body":{"dictionaryName": "const", "parserName": "simple", "text": ["This", "is", "sample", "don't", "judge", "me"]}})"_json;

QString beautify(json request) {
    return QString::fromStdString(request.dump() + "\n");
}

namespace game::client {

    GameClient::GameClient(QString name) : socket_wrapper("46.17.248.131", PORT) {
        socket_wrapper.sent(beautify(startGameRequest(name)));
        socket_wrapper.sent(beautify(joinRequest));
    }

    void GameClient::keyPressed(QChar button) {
        //socket_wrapper.sent(QString("add_symbol_%1\n").arg(button));
        local_buffer.addSymbol(button);
        socket_wrapper.sent(beautify(keyPressedRequest(local_buffer.getBuffer())));
    }

    void GameClient::backspacePressed() {
        socket_wrapper.sent(QString("add_backspace\n"));
        local_buffer.deleteSymbol();
    }

    [[nodiscard]] bool GameClient::getErrorStatus() const {
        return false;
    }
    [[nodiscard]] bool GameClient::isEnded() const {
        return false;
    }
    [[nodiscard]] QString GameClient::getBuffer() const {
        return local_buffer.getBuffer();
    }
}// namespace game::client
