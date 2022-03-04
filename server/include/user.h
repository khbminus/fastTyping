#ifndef FASTTYPING_USER_H
#define FASTTYPING_USER_H
#include <common/include/json.hpp>
#include <string>

using nlohmann::json;

namespace FastTyping::Server {
    class User {
    public:
        User(json j) : userName(j["username"]) {
            id = nextId++;// strong exception guarantee
        }
        User(const std::string &name) : userName(name) {
            id = nextId++;
        }

        const std::string &name() const {
            return userName;
        }

        const int getId() const {
            return id;
        }

    private:
        std::string userName;
        int id = 0;
        static inline int nextId = 0;
    };
}// namespace FastTyping::Server
#endif//FASTTYPING_USER_H
