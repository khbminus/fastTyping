#ifndef FASTTYPING_USER_H
#define FASTTYPING_USER_H
#include "database.h"
#include "game_fwd.h"
#include <nlohmann/json.hpp>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>

using nlohmann::json;
namespace FastTyping::Server {
    struct User {
        User() = delete;
        explicit User(std::string &name_, Database *DB) : userName(name_), id(DB->getId(name_)), gameId(-1), wantToExit(false)  {
        }
        [[nodiscard]] Game *getGame() const {
            // getGameId()
            return currentGame.get();
        }
        void setGame(std::shared_ptr<Game> game) {
            std::unique_lock l{mutex};
            currentGame = std::move(game);
            // setGameId(TODO)
        }

        [[nodiscard]] const std::string &name() const noexcept {
            return userName;
        }

        [[nodiscard]] int getId() const noexcept {
            return id;
        }

        [[nodiscard]] int isWantToExit() const noexcept {
            return (wantToExit ? 1 : 0);
        }
        void setGameId(int gameId_) noexcept {
            gameId = gameId_;
        }
        [[nodiscard]] int getGameId() const noexcept {
            return gameId;
        }
    
        void setWillToExit() noexcept {
            wantToExit = true;
        }
        void clearWillToExit() noexcept {
            wantToExit = false;
        }

    private:
        std::shared_ptr<Game> currentGame = nullptr;
        mutable std::mutex mutex;
        std::string userName;
        int gameId;
        bool wantToExit;
        int id = 0;
    };
}// namespace FastTyping::Server
#endif//FASTTYPING_USER_H
