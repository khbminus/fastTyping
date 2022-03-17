#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include "gameLogic.h"
#include "tcp_client.h"

namespace game::client {
    struct GameClient : game::AbstractGameManager  {
    private:
        web::WebClient socket_wrapper;
        QString local_buffer;
    public:
        GameClient();
        void keyPressed(QChar button) override;
        void backspacePressed() override;
        [[nodiscard]] bool getErrorStatus() const  override;
        [[nodiscard]] bool isEnded() const  override;
        [[nodiscard]] QString getBuffer() const  override;
    };
}

#endif// GAMECLIENT_H
