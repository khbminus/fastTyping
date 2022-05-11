#ifndef FASTTYPING_GAME_H
#define FASTTYPING_GAME_H
#include <abc.h>
#include <memory>
#include <mutex>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include "user.h"


namespace FastTyping::Server {
using nlohmann::json;
struct Game {
public:
    Game(std::unique_ptr<FastTyping::Logic::AbstractParser> parser_,
         std::unique_ptr<FastTyping::Logic::AbstractDictionary> dictionary_)
        : parser(std::move(parser_)), dictionary(std::move(dictionary_)) {
        id = nextId++;
    }
    [[nodiscard]] const std::string &getName() const {
        return gameName;
    }
    [[nodiscard]] int getId() const {
        return id;
    }

    bool hasUser(int uid);

    json addNewChar(int uid, char c);
    json backspace(int uid);
    json check(int uid);
    json getNewLine(int uid);
    json getStateOfUsers();

private:
    json checkUnsafe(int uid);  // THREAD UNSAFE
    bool isEndedUnsafe(int uid);

    std::string gameName;
    int id = 0;
    static inline int nextId = 0;

    std::unique_ptr<FastTyping::Logic::AbstractParser> parser;
    std::unique_ptr<FastTyping::Logic::AbstractDictionary> dictionary;

    struct AdditionalUserInfo {
        std::string currentBuffer;
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

private:
    std::mutex map_mutex;
    std::unordered_map<int, std::shared_ptr<Game>> games;
};
}  // namespace FastTyping::Server

#endif  // FASTTYPING_GAME_H
