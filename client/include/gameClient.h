#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include "gameLogic.h"
#include "tcp_client.h"

namespace game::client {

    struct GameClient : game::AbstractGameManager {
    private:
        web::WebClient socket_wrapper;
        Inputter local_buffer;
        friend struct CheckQuery;
        bool mIsEnded = false;
        bool mIsCorrect=true;
    public:
        GameClient(QString name);
        void keyPressed(QChar button) override;
        void backspacePressed() override;
        [[nodiscard]] bool getErrorStatus() const override;
        [[nodiscard]] bool isEnded() const override;
        [[nodiscard]] QString getBuffer() const override;
    };
}// namespace game::client

#endif// GAMECLIENT_H
