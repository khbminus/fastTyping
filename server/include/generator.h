#ifndef FASTTYPING_GENERATOR_H
#define FASTTYPING_GENERATOR_H
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <vector>

namespace generator {
static std::mt19937 rnd(std::time(0));
struct UsersTypo {
    char letter1, letter2;
    //        std::string word;
    //        int userId;
};
bool operator<(const UsersTypo &a, const UsersTypo &b);

struct AbstractTextGenerator {
    virtual std::vector<std::string> getTop(int k,
                                            bool isAdaptive = false,
                                            UsersTypo mistake = {'$', '$'}) = 0;
};

struct TextGenerator : AbstractTextGenerator {
private:
    std::map<UsersTypo, std::vector<std::string>>
        wordsByTypo;  // We don't care if it's not adaptive
    std::vector<std::string> words;
    std::vector<std::string> getRandom(int k);
    // cppcheck-suppress unusedPrivateFunction
    std::vector<std::string> split(std::string &corpus_line);

public:
    explicit TextGenerator(std::string &filename);
    std::vector<std::string> getTop(int k,
                                    bool isAdaptive = false,
                                    UsersTypo mistake = {'$', '$'}) override;
};
}  // namespace generator

#endif  // FASTTYPING_GENERATOR_H
