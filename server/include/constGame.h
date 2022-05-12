#ifndef FASTTYPING_CONSTGAME_H
#define FASTTYPING_CONSTGAME_H

#include <fstream>
#include <iostream>
#include <memory>
#include <thread>
#include "abc.h"

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
    [[nodiscard]] std::vector<std::string> getLine(int) const override {
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

};      // namespace FastTyping::Logic
#endif  // FASTTYPING_CONSTGAME_H
