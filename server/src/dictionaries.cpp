#include "dictionaries.h"
#include <algorithm>
#include <boost/dll/import.hpp>
#include <boost/version.hpp>
#include <cctype>
#include <iostream>
#include <sstream>
#include "boost/algorithm/string.hpp"
#include "dictionaries_api.h"

#if BOOST_VERSION >= 107600
#define boost_dll_import_symbol ::boost::dll::import_symbol
#else
#define boost_dll_import_symbol ::boost::dll::import
#endif

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
[[nodiscard]] std::size_t FileDictionary::getPrefixSize(int pos) const {
    std::size_t result = 0;
    for (int ind = 0; ind < pos; ind++) {
        result += words[ind].size() + 1;
    }
    return result;
}

DLLDictionary::DLLDictionary(std::string const &filename) {
    boost::shared_ptr<dictionary_plugin> plugin =
        boost_dll_import_symbol<dictionary_plugin>(
            filename, "dictionary", boost::dll::load_mode::append_decorations);
    words = plugin->words();
}

[[nodiscard]] std::string DLLDictionary::getWord(int index) const {
    return words[index];
}

[[nodiscard]] std::size_t DLLDictionary::getWordCount() const {
    return words.size();
}
[[nodiscard]] std::vector<std::string> DLLDictionary::getLine(int index) const {
    return words;
}
[[nodiscard]] std::size_t DLLDictionary::getLinesCount() const {
    return 1;
}

[[nodiscard]] std::size_t DLLDictionary::getPrefixSize(int pos) const {
    std::size_t result = 0;
    for (int ind = 0; ind < pos; ind++) {
        result += words[ind].size() + 1;
    }
    return result;
}

}  // namespace FastTyping::Logic
