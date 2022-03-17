#include "gameClient.h"
#include <QString>

namespace game::client {

    GameClient::GameClient() : socket_wrapper("46.17.248.131", 30566) {
    }

    void GameClient::keyPressed(QChar button) {
        socket_wrapper.sent(QString("add_symbol_%1\n").arg(button));
        local_buffer += button;
    }

    void GameClient::backspacePressed() {
        socket_wrapper.sent(QString("add_backspace\n"));
        if (local_buffer.length() != 0) {
            local_buffer.remove(local_buffer.length() - 1, 1);
        }
    }

    [[nodiscard]] bool GameClient::getErrorStatus() const {
        return false;
    }
    [[nodiscard]] bool GameClient::isEnded() const {
        return false;
    }
    [[nodiscard]] QString GameClient::getBuffer() const {
        return local_buffer;
    }
}// namespace game::client
