#ifndef FASTTYPING_ABC_H
#define FASTTYPING_ABC_H
#include <memory>
#include <string>
#include <vector>

namespace FastTyping::Logic {

struct AbstractDictionary {
    [[nodiscard]] virtual std::string getWord(int index) const = 0;
    [[nodiscard]] virtual std::size_t getWordCount() const = 0;
    [[nodiscard]] virtual std::vector<std::string> getLine(int index) const = 0;
    [[nodiscard]] virtual std::size_t getLinesCount() const = 0;
    virtual ~AbstractDictionary() = default;
};

struct AbstractParser {
    [[nodiscard]] virtual bool isFullCorrect(
        const std::string &inputWord,
        const std::string &dictionaryWord) const = 0;
    [[nodiscard]] virtual bool isPrefixCorrect(
        const std::string &inputWord,
        const std::string &dictionaryWord) const = 0;
    virtual ~AbstractParser() = default;
};
}  // namespace FastTyping::Logic

#endif  // FASTTYPING_DICTIONARY_H
