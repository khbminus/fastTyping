#ifndef FASTTYPING_USER_H
#define FASTTYPING_USER_H
#include "game_fwd.h"
#include <json.hpp>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>

using nlohmann::json;

namespace FastTyping::Server {
    class User {
    public:
        User() = default;
        explicit User(json j) : userName(j["username"]) {
            id = nextId++;// strong exception guarantee
        }
        explicit User(std::string name) : userName(std::move(name)) {
            id = nextId++;
        }

        [[nodiscard]] const std::string &name() const {
            std::unique_lock l{mutex};
            return userName;
        }

        [[nodiscard]] int getId() const noexcept {
            return id;
        }

        [[nodiscard]] int isWantToExit() const noexcept {
            return wantToExit;
        }
        [[nodiscard]] Game *getGame() const {
            std::unique_lock l{mutex};
            return currentGame.get();
        }
        void setGame(std::shared_ptr<Game> game) {
            std::unique_lock l{mutex};
            currentGame = std::move(game);
        }
        void setWillToExit() {
            std::unique_lock l{mutex};
            wantToExit = true;
        }
        void clearWillToExit() {
            std::unique_lock l{mutex};
            wantToExit = false;
        }

    private:
        std::string userName;
        int id = 0;
        bool wantToExit = false;
        static inline int nextId = 0;

        mutable std::mutex mutex;
        std::shared_ptr<Game> currentGame = nullptr;
    };

    class AbstractUserStorage {
    public:
        AbstractUserStorage() = default;
        virtual User &get(int) = 0;
        virtual User &get(const std::string &) = 0;
        virtual ~AbstractUserStorage() = default;
    };

    class MapUserStorage : public AbstractUserStorage {
        // Probably should be replaced with SQL user storage
    public:
        MapUserStorage() = default;
        User &get(int id) override {
            std::unique_lock l{mutex};
            return usersById.at(id);
        }
        User &get(const std::string &name) override {
            std::unique_lock l{mutex};
            if (auto it = usersByName.find(name); it != usersByName.end()) {
                return it->second;
            }
            User &user = usersByName.emplace(name, name).first->second;
            return usersById.emplace(user.getId(), user).first->second;
        }

    private:
        std::unordered_map<std::string, User> usersByName;
        std::unordered_map<int, User &> usersById;
        mutable std::mutex mutex;
    };
}// namespace FastTyping::Server
#endif//FASTTYPING_USER_H
