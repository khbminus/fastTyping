#include "game.h"

namespace FastTyping::Server {
[[nodiscard]] Game *User::getGame() const {
    return currentGame.get();
}
[[nodiscard]] const std::string &User::name() const noexcept {
    return userName;
}

[[nodiscard]] int User::getId() const noexcept {
    return id;
}

void User::setGame(std::shared_ptr<Game> game) {
    std::unique_lock l{mutex};
    currentGame = std::move(game);
}

void User::waitStartGame() {
    std::unique_lock l{mutex};
    if (currentGame->getHostId() != getId()) {
        currentGame->cond_gameStarted.wait(l, [this]() {
            std::cerr << "jopa\n";
            return currentGame->getGameStarted();
        });
    }
}

}  // namespace FastTyping::Server

//{"body":{}, "header":{"type":"startGame"}}