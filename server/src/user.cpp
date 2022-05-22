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

[[nodiscard]] int User::isWantToExit() const noexcept {
    std::unique_lock l{mutex};
    return (wantToExit ? 1 : 0);
}

void User::setGame(std::shared_ptr<Game> game) {
    std::unique_lock l{mutex};
    currentGame = std::move(game);
}

void User::setWillToExit() noexcept {
    std::unique_lock l{mutex};
    wantToExit = true;
}

void User::waitStartGame() {
    std::unique_lock l{mutex};
    if (currentGame->getHostId() != getId()) {
        std::cerr << "locked\n";
        currentGame->cond_gameStarted.wait(
            l, [this]() { return currentGame.get()->getGameStarted(); });
        std::cerr << "unlocked\n";
    }
}
void User::clearWillToExit() noexcept {
    wantToExit = false;
}

}  // namespace FastTyping::Server

//{"body":{}, "header":{"type":"startGame"}}