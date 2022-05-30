#include "dictionaries.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include "boost/algorithm/string.hpp"

namespace FastTyping::Logic {

std::vector<std::string> to_words(std::string raw) {
    std::vector<std::string> words;
    boost::split(words, raw, boost::is_space());
    std::for_each(words.begin(), words.end(), [](std::string &val) {
        boost::trim_if(val, boost::is_space());
    });
    words.erase(
        std::remove_if(words.begin(), words.end(),
                       [](std::string const &val) { return val.empty(); }),
        words.end());
    return words;
}

FileDictionary::FileDictionary(std::string const &filename) : file(filename) {
    std::stringstream raw;
    if (!file.is_open()) {
        words = {"Can", "not", "load", "dictionary"};
    } else {
        raw << file.rdbuf();
        words = to_words(raw.str());
    }
}

[[nodiscard]] std::string FileDictionary::getWord(int index) const {
    return words[index];
}

[[nodiscard]] std::size_t FileDictionary::getWordCount() const {
    return words.size();
}
[[nodiscard]] std::vector<std::string> FileDictionary::getLine(
    int index) const {
    return words;
}
[[nodiscard]] std::size_t FileDictionary::getLinesCount() const {
    return 1;
}
}  // namespace FastTyping::Logic
