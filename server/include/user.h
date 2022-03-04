#ifndef FASTTYPING_USER_H
#define FASTTYPING_USER_H
#include <common/include/json.hpp>
#include <string>
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
            return userName;
        }

        [[nodiscard]] int getId() const {
            return id;
        }

    private:
        std::string userName;
        int id = 0;
        static inline int nextId = 0;
    };

    class AbstractUserStorage {
        virtual User &get(int) = 0;
        virtual User &get(const std::string &) = 0;
    };

    class MapUserStorage : AbstractUserStorage {
        // Probably should be replaced with SQL user storage
    public:
        User &get(int id) override {
            return usersById.at(id);
        }
        User &get(const std::string &name) override {
            if (auto it = usersByName.find(name); it != usersByName.end()) {
                return it->second;
            }
            User user(name);

            return usersById.emplace(user.getId(),
                                     usersByName.insert({name, user}).first->second).first->second;
        }

    private:
        std::unordered_map<std::string, User> usersByName;
        std::unordered_map<int, User &> usersById;
    };
}// namespace FastTyping::Server
#endif//FASTTYPING_USER_H
