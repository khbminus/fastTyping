#ifndef FASTTYPING_CONSTGAME_H
#define FASTTYPING_CONSTGAME_H

#include <fstream>
#include <iostream>
#include <memory>
#include <thread>
#include "abc.h"
#include "generator.h"

namespace FastTyping::Logic {

struct Dictionary : AbstractDictionary {
    explicit Dictionary(std::vector<std::string> words_)
        : words(std::move(words_)) {}

    [[nodiscard]] std::string getWord(int index) const override {
        return words[index];
    }

    [[nodiscard]] size_t getWordCount() const override {
        return words.size();
    }
    [[nodiscard]] std::vector<std::string> getLine(int index) const override {
        return words;
    }
    [[nodiscard]] size_t getLinesCount() const override {
        return 1;
    }

private:
    std::vector<std::string> words;
};

struct SimpleParser : AbstractParser {
    [[nodiscard]] bool isFullCorrect(
        const std::string &inputWord,
        const std::string &dictionaryWord) const override {
        return dictionaryWord + ' ' == inputWord;
    }
    [[nodiscard]] bool isPrefixCorrect(
        const std::string &inputWord,
        const std::string &dictionaryWord) const override {
        if (inputWord.size() > dictionaryWord.size()) {
            return false;
        }
        for (std::size_t i = 0; i < inputWord.size(); i++) {
            if (inputWord[i] != dictionaryWord[i]) {
                return false;
            }
        }
        return true;
    }
};
struct AdaptiveDictionary : AbstractDictionary {
    std::vector<std::string> words;
    explicit AdaptiveDictionary(
        int user_id,
        generator::TextGenerator &gen) {  // TODO may be refactor?
        int k = 3;
        // TODO get list of user's mistakes from DB
        std::vector<generator::UsersTypo> mistakes = {{'a', 'b'}, {'c', 'd'}};
        for (auto cur_mistake : mistakes) {
            std::vector<std::string> cur_words =
                gen.getTop(k, true, cur_mistake);
            for (auto x : cur_words)
                words.emplace_back(x);
        }
    }

    [[nodiscard]] std::string getWord(int index) const override {
        return words[index];
    }

    [[nodiscard]] size_t getWordCount() const override {
        return words.size();
    }
    [[nodiscard]] std::vector<std::string> getLine(int index) const override {
        return words;
    }
    [[nodiscard]] std::size_t getLinesCount() const override {
        return 1;  // TODO lol what???
    }
};
}  // namespace FastTyping::Logic
// namespace FastTyping::Logic
#endif  // FASTTYPING_CONSTGAME_H
