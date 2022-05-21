#include "generator.h"
#include <sstream>

// How to work with:
// 1) On server create an object of TextGenerator using filename of
// dictionary.txt 2) 1 line == 1 word, so in case of quotes etc you need to
// parse it later..??? you can use split method? 3) Use getTop k, to get k LINES
// of dictionary

namespace generator {
bool operator<(const UsersTypo &a, const UsersTypo &b) {
    if (a.letter1 == b.letter1)
        return a.letter2 < b.letter2;
    return a.letter1 < b.letter1;
}
std::vector<std::string> TextGenerator::getRandom(int k) {
    std::vector<std::string> result(k);
    for (int ind = 0; ind < k; ind++) {
        result[ind] = words[rnd() % words.size()];
    }
    return result;
}

std::vector<std::string> TextGenerator::split(std::string &corpus_line) {
    std::vector<std::string> res;
    std::stringstream ss(corpus_line);
    std::string cur;
    while (ss >> cur) {
        res.push_back(cur);
    }
    return res;
}

TextGenerator::TextGenerator(std::string &filename) {
    std::fstream in(filename, std::ios::in);
    std::string corpus_line;
    while (std::getline(in, corpus_line)) {
        for (int ind = 1; ind < corpus_line.size(); ind++) {
            wordsByTypo[{corpus_line[ind - 1], corpus_line[ind]}].push_back(
                corpus_line);
        }
        words.push_back(corpus_line);
    }
}

std::vector<std::string> TextGenerator::getTop(int k,
                                               bool isAdaptive,
                                               UsersTypo mistake) {
    if (!isAdaptive) {  // if it's not adaptive return random words
        return getRandom(k);
    }
    std::vector<std::string> result(k);
    if (wordsByTypo[mistake].size() < k) {
        std::cerr << "ALERT! not enough words with such a mistake";
    }
    for (int ind = 0; ind < k; ind++) {
        result[ind] = wordsByTypo[mistake][rnd() % wordsByTypo[mistake].size()];
    }
    return result;
}
}  // namespace generator