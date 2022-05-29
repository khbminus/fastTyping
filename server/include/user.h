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
    User(std::string name_, Database *DB)
        : userName(std::move(name_)), id(DB->getId(name_)) {}
    [[nodiscard]] Game *getGame() const;
    [[nodiscard]] const std::string &name() const noexcept;
    [[nodiscard]] int getId() const noexcept;
    void setGame(std::shared_ptr<Game> game);
    void waitStartGame();

private:
    std::shared_ptr<Game> currentGame = nullptr;
    mutable std::mutex mutex;
    std::string userName;
    int id = 0;
};
}  // namespace FastTyping::Server
#endif  // FASTTYPING_USER_H
