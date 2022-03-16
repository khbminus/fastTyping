#ifndef FASTTYPING_GAME_H
#define FASTTYPING_GAME_H
#include "user.h"
#include <abc.h>
#include <json.hpp>
#include <memory>
#include <string>
#include <unordered_map>

using nlohmann::json;

namespace FastTyping::Server {
    struct Game {
    public:
        Game(std::unique_ptr<FastTyping::Logic::AbstractParser> parser_,
             std::unique_ptr<FastTyping::Logic::AbstractDictionary> dictionary_) : parser(std::move(parser_)),
                                                                                   dictionary(std::move(dictionary_)) {
            id = nextId++;
        }
        [[nodiscard]] const std::string &getName() const {
            return gameName;
        }
        [[nodiscard]] int getId() const {
            return id;
        }

        json checkInputAndProceed(const User &user, json queryBody);
        json getNewLine(const User &user, json queryBody);


    private:
        std::string gameName;
        int id = 0;
        static inline int nextId = 0;

        std::unique_ptr<FastTyping::Logic::AbstractParser> parser;
        std::unique_ptr<FastTyping::Logic::AbstractDictionary> dictionary;

        struct AdditionalUserInfo {
            int currentWord = 0;
            int lineNumber = 0;
        };

        std::unordered_map<int, AdditionalUserInfo> additionalInfo;
    };

    std::unique_ptr<Game> makeGame(User& user, json body);
}// namespace FastTyping::Server


#endif//FASTTYPING_GAME_H
