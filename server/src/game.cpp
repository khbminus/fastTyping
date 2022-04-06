#include "game.h"
#include "constGame.h"

namespace FastTyping::Server {
    json Game::checkInputAndProceed(const User &user, json queryBody) {
        std::unique_lock l{mutex};
        std::string userWord = queryBody["word"];
        std::string rightWord = dictionary->getWord(additionalInfo[user.getId()].currentWord);
        json result;
        result["header"] = {{"type", "checkResult"}};
        if (parser->isCorrect(userWord, rightWord)) {
            int currentWord = ++additionalInfo[user.getId()].currentWord;
            result["body"] = {{{"isCorrect", true}}, {{"isEnd", (currentWord == dictionary->getWordCount())}}};
            return result;
        }
        result["body"] = {{"isCorrect", false}, {"isEnd", nullptr}};
        return result;
    }
    json Game::getNewLine(const User &user, json) {
        std::unique_lock l{mutex};
        int currentLine = additionalInfo[user.getId()].lineNumber++;
        std::string line = dictionary->getLine(currentLine);
        return {
                {"header", {{"type", "newLineResult"}}},
                {"body", {{"line", line}}}};
    }

    json Game::getStateOfUsers() {
        std::unique_lock l{mutex};
        json result = {{"header", {{"type", "currentState"}}}};
        json &userStates = result["body"]["userStates"];
        for (const auto &[uid, info]: additionalInfo) {
            userStates.emplace_back();
            userStates.back()["id"] = uid;
            userStates.back()["wordsTyped"] = info.currentWord;
            userStates.back()["linesTyped"] = info.lineNumber;
        }
        return result;
    }

    std::shared_ptr<Game> MapGameStorage::get(int id, json &errors) {
        if (auto it = games.find(id); it != games.end())
            return it->second;
        errors = {{"header", {{"type", "error"}}}, {"body", {{"text", "Can't find game with specific id"}}}};
        return nullptr;
    }
    json MapGameStorage::createGame(const json &body) {
        if (body["dictionaryName"] != "const" || body["parserName"] != "simple") {
            return {{"header", {{"type", "error"}}}, {"body", {{"text", "wrong parameters"}}}};
        }
        if (!body["words"].is_array()) {
            return {{"header", {{"type", "error"}}}, {"body", {{"text", "Can't find words"}}}};
        }
        std::shared_ptr<Game> game = std::make_shared<Game>(std::make_unique<Logic::SimpleParser>(),
                                                            std::make_unique<Logic::Dictionary>(
                                                                    body["words"].get<std::vector<std::string>>()));
        games[game->getId()] = game;
        return {{"header", {{"type", "GameCreatedSuccessfully"}}}, {"body", {{"id", game->getId()}}}};
    }
}// namespace FastTyping::Server