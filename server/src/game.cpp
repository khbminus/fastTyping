#include "game.h"
#include <algorithm>
#include <boost/locale.hpp>
#include <boost/log/trivial.hpp>
#include <iostream>
#include "constGame.h"

namespace FastTyping::Server {
bool Game::hasUser(int uid) {
    std::unique_lock l{mutex};
    return static_cast<bool>(additionalInfo.count(uid));
}

bool Game::isEndedUnsafe(int uid) {
    return additionalInfo[uid].currentWord == dictionary->getWordCount();
}

json Game::checkUnsafe(int uid) {
    json result;
    std::string rightWord =
        dictionary->getWord(additionalInfo[uid].currentWord);
    auto &word = additionalInfo[uid].currentBuffer;
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

json Game::check(int uid) {
    std::unique_lock l{mutex};
    return checkUnsafe(uid);
}

json Game::addNewChar(int uid, const std::string &c) {
    std::unique_lock l{mutex};
    additionalInfo[uid].currentBuffer.push_back(c);
    auto checkResult = checkUnsafe(uid);
    if (checkResult["body"]["isFullCorrect"] == true) {
        auto &info = additionalInfo[uid];
        info.currentBuffer.clear();
        info.currentWord++;
        checkResult["body"]["isEnd"] = isEndedUnsafe(uid);
        return checkResult;
    }
    return checkResult;
}
json Game::backspace(int uid) {
    std::unique_lock l{mutex};
    auto &word = additionalInfo[uid].currentBuffer;
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

std::shared_ptr<Game> MapGameStorage::get(int id, json &errors) {
    std::unique_lock l{map_mutex};
    if (auto it = games.find(id); it != games.end()) {
        return it->second;
    }
    errors = {{"header", {{"type", "wrongIdError"}}},
              {"body", {{"text", "Can't find game with specific id"}}}};
    return nullptr;
}
json MapGameStorage::createGame(const json &body) {
    if (body["dictionaryName"] != "const" || body["parserName"] != "simple") {
        return {{"header", {{"type", "wrongFormatError"}}},
                {"body", {{"text", "wrong parameters"}}}};
    }
    if (!body.contains("words") || !body["words"].is_array()) {
        return {{"header", {{"type", "wrongFormatError"}}},
                {"body", {{"text", "Can't find words"}}}};
    }
    auto words = body["words"].get<std::vector<std::string>>();
    std::for_each(words.begin(), words.end(), [&](std::string &word) {
        word = boost::locale::normalize(word, boost::locale::norm_nfkc);
    });
    std::shared_ptr<Game> game =
        std::make_shared<Game>(std::make_unique<Logic::SimpleParser>(),
                               std::make_unique<Logic::Dictionary>(words));
    {
        std::unique_lock l{map_mutex};
        games[game->getId()] = game;
    }
    return {{"header", {{"type", "GameCreatedSuccessfully"}}},
            {"body", {{"id", game->getId()}}}};
}
}  // namespace FastTyping::Server