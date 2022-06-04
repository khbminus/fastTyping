#ifndef FASTTYPING_GAME_H
#define FASTTYPING_GAME_H
#include <abc.h>
#include <chrono>
#include <condition_variable>
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
         std::unique_ptr<FastTyping::Logic::AbstractDictionary> dictionary_,
         int hostId_,
         std::string &dictName,
         int game_id)
        : parser(std::move(parser_)),
          dictionary(std::move(dictionary_)),
          hostId(hostId_),
          dictName(dictName),
          id(game_id){};
    [[nodiscard]] const std::string &getName() const {
        return gameName;
    }
    [[nodiscard]] int getId() const {
        return id;
    }

    bool hasUser(int uid);
    bool getGameStarted();
    int getHostId();
    void startGame();
    void userFinished(int uid);

    // character is a string due to UTF-8
    json addNewChar(int uid, const std::string &c);
    json backspace(int uid);
    json check(int uid);
    json getNewLine(int uid);
    json getStateOfUsers();
    json getStatistics(int uid);
    void joinUser(int uid);

    std::condition_variable cond_gameStarted;

private:
    json checkUnsafe(int uid);  // THREAD UNSAFE
    bool isEndedUnsafe(int uid);

    std::string gameName;
    int hostId;
    int id = 0;
    static inline int nextId = 0;
    std::optional<std::chrono::high_resolution_clock::time_point> gameStartTime;
    std::unique_ptr<FastTyping::Logic::AbstractParser> parser;
    std::unique_ptr<FastTyping::Logic::AbstractDictionary> dictionary;
    std::string dictName;

    struct AdditionalUserInfo {
        std::vector<std::string> currentBuffer;
        int currentWord = 0;
        int lineNumber = 0;
        // statistics
        int correctChars = 0;
        int totalChars = 0;
        double finishTime = -1;
    };

    mutable std::mutex mutex;
    std::unordered_map<int, AdditionalUserInfo> additionalInfo;
};

class AbstractGameStorage {
public:
    AbstractGameStorage() = default;
    virtual std::shared_ptr<Game> get(int id, json &errors) = 0;
    virtual json createGame(const json &body, int host_id, bool adapt) = 0;
    virtual Game *getGame(int game_id) = 0;
    virtual ~AbstractGameStorage() = default;
};

class MapGameStorage final : public AbstractGameStorage {
public:
    std::shared_ptr<Game> get(int id, json &errors) override;
    json createGame(const json &body, int host_id, bool adapt = false) override;
    Game *getGame(int game_id) override {
        return games[game_id].get();
    }

private:
    std::mutex map_mutex;
    std::unordered_map<int, std::shared_ptr<Game>> games;
};
}  // namespace FastTyping::Server

#endif  // FASTTYPING_GAME_H
