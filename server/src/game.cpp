#include "game.h"
#include <boost/log/trivial.hpp>
#include <iostream>
#include <ratio>
#include "constGame.h"

namespace FastTyping::Server {
bool Game::hasUser(int uid) {
    std::unique_lock l{mutex};
    return static_cast<bool>(additionalInfo.count(uid));
}

bool Game::isEndedUnsafe(int uid) {
    return additionalInfo[uid].currentWord == dictionary->getWordCount();
}

bool Game::getGameStarted() {
    std::unique_lock l{mutex};
    return gameStartTime.has_value();
}

int Game::getHostId() {
    std::unique_lock l{mutex};
    return hostId;
}

void Game::startGame() {
    gameStartTime = std::chrono::high_resolution_clock::now();
    cond_gameStarted.notify_all();
}

void Game::userFinished(int uid) {
    using namespace std::chrono;
    assert(gameStartTime.has_value());
    duration<double> time_span = duration_cast<duration<double>>(
        high_resolution_clock::now() - gameStartTime.value());
    additionalInfo[uid].finishTime = time_span.count();
    std::cerr << "Finis time uf user " << uid << " "
              << additionalInfo[uid].finishTime << '\n';
}

json Game::checkUnsafe(int uid) {
    json result;
    std::string rightWord =
        dictionary->getWord(additionalInfo[uid].currentWord);
    std::string &word = additionalInfo[uid].currentBuffer;
    BOOST_LOG_TRIVIAL(debug)
        << "Buffer of user " << uid << " is \"" << word << "\"\n";
    result["header"] = {{"type", "checkResult"}};
    result["body"] = {
        {"isFullCorrect", parser->isFullCorrect(word, rightWord)},
        {"isPrefixCorrect", parser->isPrefixCorrect(word, rightWord)},
        {"isEnd", false},
    };
    return result;
}

json Game::check(int uid) {
    std::unique_lock l{mutex};
    return checkUnsafe(uid);
}

json Game::addNewChar(int uid, char c) {
    std::unique_lock l{mutex};
    additionalInfo[uid].currentBuffer += c;
    additionalInfo[uid].totalChars++;
    auto checkResult = checkUnsafe(uid);
    if (checkResult["body"]["isPrefixCorrect"] == true ||
        checkResult["body"]["isFullCorrect"] == true) {  // space case
        additionalInfo[uid].correctChars++;
    }
    if (checkResult["body"]["isFullCorrect"] == true) {
        auto &info = additionalInfo[uid];
        info.currentBuffer.clear();
        info.currentWord++;
        checkResult["body"]["isEnd"] = isEndedUnsafe(uid);
        if (isEndedUnsafe(uid)) {
            userFinished(uid);
            std::cerr << "user finished\n";
        }
        return checkResult;
    }
    return checkResult;
}
json Game::backspace(int uid) {
    std::unique_lock l{mutex};
    std::string &word = additionalInfo[uid].currentBuffer;
    additionalInfo[uid].totalChars++;
    auto checkResult = checkUnsafe(uid);
    if (checkResult["body"]["isPrefixCorrect"] == true) {
        additionalInfo[uid].correctChars--;
    }
    if (word.empty()) {
        return {{"header", {{"type", "emptyBufferError"}}},
                {"body", {{"text", "can't use backspace with empty buffer"}}}};
    }
    word.pop_back();
    return checkUnsafe(uid);
}

json Game::getNewLine(int uid) {
    std::unique_lock l{mutex};
    int currentLine = additionalInfo[uid].lineNumber++;
    auto line = dictionary->getLine(currentLine);
    return {{"header", {{"type", "newLineResult"}}},
            {"body", {{"line", line}}}};
}

json Game::getStateOfUsers() {
    std::unique_lock l{mutex};
    json result = {{"header", {{"type", "currentState"}}}};
    json &userStates = result["body"]["userStates"];
    // cppcheck-suppress unassignedVariable
    for (const auto &[uid, info] : additionalInfo) {
        userStates.emplace_back();
        userStates.back()["id"] = uid;
        userStates.back()["wordsTyped"] = info.currentWord;
        userStates.back()["linesTyped"] = info.lineNumber;
    }
    return result;
}
json Game::getStatistics(int uid) {
    std::unique_lock l{mutex};
    json result = {{"header", {{"type", "GameStatistics"}}}};
    double convertToWpm = 60.0 / additionalInfo[uid].finishTime / 4;
    // TODO replace 4 with average word length
    result["body"]["wpm"] = additionalInfo[uid].correctChars * convertToWpm;
    result["body"]["rawWmp"] = additionalInfo[uid].totalChars * convertToWpm;
    result["body"]["correctChars"] = additionalInfo[uid].correctChars;
    result["body"]["totalChars"] = additionalInfo[uid].totalChars;

    return result;
}
std::shared_ptr<Game> MapGameStorage::get(int id, json &errors) {
    std::unique_lock l{map_mutex};
    if (auto it = games.find(id); it != games.end())
        return it->second;
    errors = {{"header", {{"type", "wrongIdError"}}},
              {"body", {{"text", "Can't find game with specific id"}}}};
    return nullptr;
}

json MapGameStorage::createGame(
    const json &body,
    std::unique_ptr<FastTyping::Logic::AbstractDictionary> dictionary,
    int host_id) {
    if (body["parserName"] != "simple") {
        return {{"header", {{"type", "wrongFormatError"}}},
                {"body", {{"text", "wrong parameters"}}}};
    }
    if (!body.contains("words") || !body["words"].is_array()) {
        return {{"header", {{"type", "wrongFormatError"}}},
                {"body", {{"text", "Can't find words"}}}};
    }
    std::shared_ptr<Game> game =
        std::make_shared<Game>(std::make_unique<Logic::SimpleParser>(),
                               std::move(dictionary), host_id);
    {
        std::unique_lock l{map_mutex};
        games[game->getId()] = game;
    }
    return {{"header", {{"type", "GameCreatedSuccessfully"}}},
            {"body", {{"id", game->getId()}}}};
}
}  // namespace FastTyping::Server