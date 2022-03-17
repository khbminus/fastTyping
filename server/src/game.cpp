#include "game.h"
#include "constGame.h"

namespace FastTyping::Server {
    json Game::checkInputAndProceed(const User &user, json queryBody) {
        std::string userWord = queryBody["word"];
        std::string rightWord = dictionary->getWord(additionalInfo[user.getId()].currentWord);
        json result;
        result["header"] = {{"type", "checkResult"}};
        if (parser->isCorrect(userWord, rightWord)) {
            int currentWord = ++additionalInfo[user.getId()].currentWord;
            result["body"] = {{"isCorrect", true}, {"isEnd", (currentWord == dictionary->getWordCount())}};
            return result;
        }
        result["body"] = {{"isCorrect", false}, {"isEnd", nullptr}};
        return result;
    }
    json Game::getNewLine(const User &user, json) {
        int currentLine = additionalInfo[user.getId()].lineNumber++;
        std::string line = dictionary->getLine(currentLine);
        return {
                {"header", {{"type", "newLineResult"}}},
                {"body", {{"line", line}}}};
    }

    std::unique_ptr<Game> makeGame(User &user, json body, json &error) {
        if (!body["dictionaryName"].is_string()) {
            error = {{"header", {{"type", "error"}}}, {"body", {{"text", "can't find \"dictionaryName\""}}}};
            return nullptr;
        }

        if (!body["parserName"].is_string()) {
            error = {{"header", {{"type", "error"}}}, {"body", {{"text", "can't find \"parserName\""}}}};
            return nullptr;
        }
        if (body["dictionaryName"] != "const" || body["parserName"] != "simple") {
            error = {{"header", {{"type", "error"}}}, {"body", {{"text", "wrong parameters"}}}};
            return nullptr;
        }
        return std::make_unique<Game>(
                std::make_unique<FastTyping::Logic::SimpleParser>(),
                std::make_unique<FastTyping::Logic::Dictionary>(body["text"]));
    }


}// namespace FastTyping::Server