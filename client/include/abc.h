//
// Created by doktorkrab on 2/20/22.
//

#ifndef FASTTYPING_ABC_H
#define FASTTYPING_ABC_H
#include <string>
#include <vector>
#include <memory>

struct AbstractDictionary {
    [[nodiscard]] virtual std::string getCurrentWord() const;
    virtual void nextWord();
};

struct AbstractOutputter {
    virtual void printSymbol(char) = 0;
    virtual void highlightError(std::size_t position) = 0;
    virtual void highlightPositions(std::size_t howMany);
};

struct AbstractInputter {
    virtual char getSymbol() = 0;
    virtual std::string getBuffer() = 0;
    virtual void clearBuffer() = 0;
    virtual bool endOfInput() const = 0;
};

struct AbstractParser {
    virtual bool isCorrect(const std::string &inputWord, const std::string &dictionaryWord) const;
};

struct AbstractDictionaryLoader {
    virtual std::unique_ptr<AbstractDictionary> loadDictionary(const std::string &name) = 0;
};


#endif//FASTTYPING_DICTIONARY_H
