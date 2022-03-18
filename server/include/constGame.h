#ifndef FASTTYPING_CONSTGAME_H
#define FASTTYPING_CONSTGAME_H

#include "abc.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <thread>

namespace FastTyping::Logic {

    struct Dictionary : AbstractDictionary {
        std::vector<std::string> words;
        std::size_t currentPosition = 0;
        explicit Dictionary(std::vector<std::string> words_) : words(std::move(words_)) {}

        [[nodiscard]] std::string getWord(int index) const override {
            return words[index];
        }

        [[nodiscard]] size_t getWordCount() const override {
            return words.size();
        }
        [[nodiscard]] std::string getLine(int index) const override {
            std::string result;
            for (const std::string &s: words)
                result += s + ' ';
            if (!result.empty())
                result.pop_back();
            return result;
        }
        [[nodiscard]] size_t getLinesCount() const override {
            return 1;
        }
    };

    struct SimpleParser : AbstractParser {

        [[nodiscard]] bool isCorrect(const std::string &inputWord, const std::string &dictionaryWord) const override {
            return dictionaryWord == inputWord;
        }
    };


};    // namespace FastTyping::Logic
#endif// FASTTYPING_CONSTGAME_H
