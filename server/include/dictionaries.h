#ifndef DICTIONARIES_H
#define DICTIONARIES_H

#include <fstream>
#include "abc.h"
#include "dictfwd.h"

namespace FastTyping::Logic {

std::vector<std::string> to_words(std::string raw);

struct FileDictionary : AbstractDictionary {
    explicit FileDictionary(std::string const &filename);
    [[nodiscard]] std::string getWord(int index) const override;
    [[nodiscard]] size_t getWordCount() const override;
    [[nodiscard]] std::vector<std::string> getLine(int index) const override;
    [[nodiscard]] size_t getLinesCount() const override;
    [[nodiscard]] std::size_t getPrefixSize(int pos) const override;

private:
    std::vector<std::string> words;
    std::ifstream file;
};

struct DLLDictionary : AbstractDictionary {
    explicit DLLDictionary(std::string const &filename);
    [[nodiscard]] std::string getWord(int index) const override;
    [[nodiscard]] size_t getWordCount() const override;
    [[nodiscard]] std::vector<std::string> getLine(int index) const override;
    [[nodiscard]] size_t getLinesCount() const override;
    [[nodiscard]] std::size_t getPrefixSize(int pos) const override;

private:
    std::vector<std::string> words;
};

struct CorpusDictionary : AbstractDictionary {
    explicit CorpusDictionary(std::string const &corpus,
                              bool adapt = false,
                              int user_id = -1);
    [[nodiscard]] std::string getWord(int index) const override;
    [[nodiscard]] size_t getWordCount() const override;
    [[nodiscard]] std::vector<std::string> getLine(int index) const override;
    [[nodiscard]] size_t getLinesCount() const override;
    [[nodiscard]] std::size_t getPrefixSize(int pos) const override;

private:
    std::vector<std::string> words;
};

void add_corpus_dictionary(std::string const &name,
                           std::vector<std::string> words);

}  // namespace FastTyping::Logic

#endif  // FASTTYPING_DATABASE_H
