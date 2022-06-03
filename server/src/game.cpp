#include "game.h"
#include <algorithm>
#include <boost/locale.hpp>
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
    auto &word = additionalInfo[uid].currentBuffers.back();
    BOOST_LOG_TRIVIAL(debug) << "Buffer of user " << uid << " is \"";
    for (auto chr : word) {
        BOOST_LOG_TRIVIAL(debug) << chr;
    }
    BOOST_LOG_TRIVIAL(debug) << "\"";
    auto concatenatedBuffer =
        std::accumulate(word.begin(), word.end(), std::string());

    result["header"] = {{"type", "checkResult"}};
    result["body"] = {
        {"isFullCorrect", parser->isFullCorrect(concatenatedBuffer, rightWord)},
        {"isPrefixCorrect",
         parser->isPrefixCorrect(concatenatedBuffer, rightWord)},
        {"isEnd", false},
    };
    return result;
}

std::size_t Game::getCntCorrect(int uid) {
    std::size_t curr_word = additionalInfo[uid].currentWord;
    std::size_t cnt = 0;
    while (!additionalInfo[uid].currentBuffers.empty()) {
        std::string rightWord =
            dictionary->getWord(additionalInfo[uid].currentWord--) + ' ';
        auto &word = additionalInfo[uid].currentBuffers.back();

        BOOST_LOG_TRIVIAL(debug) << "Buffer of user " << uid << " is \"";
        for (auto chr : word) {
            BOOST_LOG_TRIVIAL(debug) << chr;
        }
        BOOST_LOG_TRIVIAL(debug) << "\"";
        auto concatenatedBuffer =
            std::accumulate(word.begin(), word.end(), std::string());
        std::size_t pos = 0;
        while (pos < concatenatedBuffer.size() && pos < rightWord.size()) {
            if (concatenatedBuffer[pos] == rightWord[pos]) {
                cnt++;
            }
            pos++;
        }
        additionalInfo[uid].currentBuffers.pop_back();
    }
    additionalInfo[uid].currentBuffers.push_back({});
    additionalInfo[uid].currentWord = curr_word;
    return cnt;
}

json Game::check(int uid) {
    std::unique_lock l{mutex};
    return checkUnsafe(uid);
}

json Game::addNewChar(int uid, const std::string &c) {
    std::unique_lock l{mutex};
    additionalInfo[uid].totalChars++;
    additionalInfo[uid].currentBuffers.back().push_back(c);
    auto checkResult = checkUnsafe(uid);
    if (checkResult["body"]["isFullCorrect"] == true) {
        auto &info = additionalInfo[uid];
        info.correctChars += getCntCorrect(uid);
        info.currentWord++;
        checkResult["body"]["isEnd"] = isEndedUnsafe(uid);
        if (isEndedUnsafe(uid)) {
            userFinished(uid);
        }
        return checkResult;
    } else if (isSolo && c == " " && additionalInfo[uid].currentBuffers.size() !=
                               dictionary->getWordCount() - 1) {
        additionalInfo[uid].currentBuffers.push_back({});
        additionalInfo[uid].currentWord++;
    }
    return checkResult;
}

json Game::backspace(int uid) {
    std::unique_lock l{mutex};
    auto &word = additionalInfo[uid].currentBuffers.back();
    additionalInfo[uid].totalChars++;
    if (word.empty()) {
        if (additionalInfo[uid].currentBuffers.size() == 1) {
            return {
                {"header", {{"type", "emptyBufferError"}}},
                {"body", {{"text", "can't use backspace with empty buffer"}}}};
        }
        assert(isSolo);
        additionalInfo[uid].currentWord--;
        additionalInfo[uid].currentBuffers.pop_back();
        word = additionalInfo[uid].currentBuffers.back();
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
        int symbolsTyped = dictionary->getPrefixSize(info.currentWord);

        if (dictionary->getWordCount() != info.currentWord) {
            auto word = dictionary->getWord(info.currentWord);
            auto bufferAsString = std::accumulate(
                info.currentBuffers.back().begin(),
                info.currentBuffers.back().end(), std::string());
            symbolsTyped +=
                parser->getCorrectPrefixLength(bufferAsString, word);
        }
        userStates.back()["symbolsTyped"] = symbolsTyped;
    }
    return result;
}
json Game::getStatistics(int uid) {
    std::unique_lock l{mutex};
    json result = {{"header", {{"type", "GameStatistics"}}}};
    double convertToWpm = 60.0 / additionalInfo[uid].finishTime / 4;
    // TODO replace 4 with average word length
    result["body"]["WPM"] = additionalInfo[uid].correctChars * convertToWpm;
    result["body"]["rawWPM"] = additionalInfo[uid].totalChars * convertToWpm;
    result["body"]["correctChars"] = additionalInfo[uid].correctChars;
    result["body"]["totalChars"] = additionalInfo[uid].totalChars;

    return result;
}

void Game::joinUser(int uid) {
    std::unique_lock l{mutex};
    additionalInfo[uid] = {};
}

void Game::setSolo() {
    std::unique_lock l{mutex};
    isSolo = true;
}

std::shared_ptr<Game> MapGameStorage::get(int id, json &errors) {
    std::unique_lock l{map_mutex};
    if (auto it = games.find(id); it != games.end()) {
        return it->second;
    }
    errors = {{"header", {{"type", "wrongIdError"}}},
              {"body", {{"text", "Can't find game with specific id"}}}};
    return nullptr;
}

json MapGameStorage::createGame(
    const json &body,
    std::unique_ptr<FastTyping::Logic::AbstractDictionary> dictionary,
    int host_id) {
    if (body["parserName"] != "simple" && body["parserName"] != "solo") {
        return {{"header", {{"type", "wrongFormatError"}}},
                {"body", {{"text", "wrong parameters"}}}};
    }
    if (!body.contains("words") || !body["words"].is_array()) {
        return {{"header", {{"type", "wrongFormatError"}}},
                {"body", {{"text", "Can't find words"}}}};
    }
    std::shared_ptr<Game> game;
    if (body["parserName"] == "simple") {
        game = std::make_shared<Game>(std::make_unique<Logic::SimpleParser>(),
                                      std::move(dictionary), host_id);
    } else {
        game = std::make_shared<Game>(std::make_unique<Logic::SoloParser>(),
                                      std::move(dictionary), host_id);
        game->setSolo();
    }

    {
        std::unique_lock l{map_mutex};
        games[game->getId()] = game;
    }
    return {{"header", {{"type", "GameCreatedSuccessfully"}}},
            {"body", {{"id", game->getId()}}}};
}
}  // namespace FastTyping::Server