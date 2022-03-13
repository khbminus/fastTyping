#ifndef FASTTYPING_CONSTGAME_H
#define FASTTYPING_CONSTGAME_H

#include "gameLogic.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <thread>

struct Dictionary : AbstractDictionary {
    std::vector<std::string> words;
    std::size_t currentPosition = 0;
    explicit Dictionary(std::vector<std::string> words_) : words(std::move(words_)) {}

    [[nodiscard]] std::string getCurrentWord() const override {
        return words.at(currentPosition);
    }

    void nextWord() override {
        currentPosition++;
    }
};

struct ConstInputter : AbstractInputter {
    std::string data;
    std::string buffer;
    std::size_t cur = 0;
    explicit ConstInputter(std::string d) : data(std::move(d)) {}

    char getSymbol() override {
        if (data[cur] != ' ') {
            buffer.push_back(data[cur]);
        }
        return data[cur++];
    }

    [[nodiscard]] bool endOfInput() const override {
        return cur == data.length();
    }

    std::string getBuffer() override {
        return buffer;
    }

    void clearBuffer() override {
        buffer.clear();
    }
};

struct SimpleParser : AbstractParser {

    [[nodiscard]] bool isCorrect(const std::string &inputWord, const std::string &dictionaryWord) const override {
        return dictionaryWord == inputWord;
    }
};

struct FileDictionaryLoader : AbstractDictionaryLoader {
    std::unique_ptr<AbstractDictionary> loadDictionary(const std::string &name) override {
        std::fstream dictionaryFile(name);
        std::string word;
        std::vector<std::string> dictionary;
        while (dictionaryFile >> word)
            dictionary.push_back(word);
        return std::unique_ptr<AbstractDictionary>(new Dictionary(dictionary));
    }
};

struct ConsoleOutputter : AbstractOutputter {
    inline static const std::string ESCAPE_CODE = "\x1b";
    ConsoleOutputter() {
        std::cout << "\n"
                  << ESCAPE_CODE << "[1A";
    }
    std::size_t currentPosition = 0;
    void printSymbol(char i) override {
        std::cout << i;
        currentPosition++;
    }
    void highlightError(std::size_t position) override {
        moveCursorLeft(currentPosition - position);
        std::cout << ESCAPE_CODE << "[1B" << 'W';
        std::cout << ESCAPE_CODE << "[1A";
        moveCursorLeft(1);
        moveCursorRight(currentPosition - position);
    }

    void highlightPositions(std::size_t howMany) override {
        moveCursorLeft(howMany);
        std::cout << ESCAPE_CODE << "[1B";
        for (int i = 0; i < howMany; i++)
            std::cout << 'W';
        std::cout << ESCAPE_CODE << "[1A";
    }
    ~ConsoleOutputter() {
        std::cout << ESCAPE_CODE << "[1B";
    }

private:
    void moveCursorLeft(std::size_t x) {
        std::cout << ESCAPE_CODE + "[" << x << "D";
    }
    void moveCursorRight(std::size_t x) {
        std::cout << ESCAPE_CODE + "[" << x << "C";
    }
};

struct FinishWordUseCase {
    std::unique_ptr<AbstractDictionary> dictionary;
    std::unique_ptr<AbstractOutputter> outputter;
    std::unique_ptr<AbstractInputter> inputter;
    std::unique_ptr<AbstractParser> parser;

    void execute() {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        bool isWordCorrect = parser->isCorrect(inputter->getBuffer(), dictionary->getCurrentWord());
        if (!isWordCorrect) {
            auto newSymbolPosition = inputter->getBuffer().size();
            outputter->highlightPositions(newSymbolPosition);
        }
        dictionary->nextWord();
        inputter->clearBuffer();
    }
};

#endif// FASTTYPING_CONSTGAME_H
