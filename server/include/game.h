#ifndef FASTTYPING_GAME_H
#define FASTTYPING_GAME_H
#include "user.h"
#include <abc.h>
#include <json.hpp>
#include <memory>
#include <mutex>
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
        json getStateOfUsers();


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

        mutable std::mutex mutex;
        std::unordered_map<int, AdditionalUserInfo> additionalInfo;
    };

    class AbstractGameStorage {
    public:
        AbstractGameStorage() = default;
        virtual std::shared_ptr<Game> get(int id, json &errors) = 0;
        virtual json createGame(const json &body) = 0;
        virtual ~AbstractGameStorage() = default;
    };

    class MapGameStorage final : public AbstractGameStorage {
    public:
        std::shared_ptr<Game> get(int id, json &errors) override;
        json createGame(const json &body) override;
        std::mutex map_mutex;

    private:
        std::unordered_map<int, std::shared_ptr<Game>> games;
    };
}// namespace FastTyping::Server


#endif//FASTTYPING_GAME_H
