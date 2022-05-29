#ifndef FASTTYPING_USER_H
#define FASTTYPING_USER_H
#include <nlohmann/json.hpp>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include "database.h"
#include "game_fwd.h"

using nlohmann::json;
namespace FastTyping::Server {
struct User {
    User() = delete;
    explicit User(std::string &name_, UserStorage &DB)
        : userName(name_), id(DB.getId(name_)), gameId(-1), wantToExit(false) {}
    [[nodiscard]] Game *getGame() const;
    [[nodiscard]] const std::string &name() const noexcept;
    [[nodiscard]] int getId() const noexcept;
    [[nodiscard]] int isWantToExit() const noexcept;
    void setGame(std::shared_ptr<Game> game);
    void setWillToExit() noexcept;
    void clearWillToExit() noexcept;
    void waitStartGame();

private:
    std::shared_ptr<Game> currentGame = nullptr;
    mutable std::mutex mutex;
    std::string userName;
    int gameId;
    bool wantToExit;
    int id = 0;
};
}  // namespace FastTyping::Server
#endif  // FASTTYPING_USER_H
