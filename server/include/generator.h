#ifndef FASTTYPING_GENERATOR_H
#define FASTTYPING_GENERATOR_H
#include <map>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <iostream>

namespace generator{
    std::mt19937 rnd(std::chrono::steady_clock::now().time_since_epoch().count());
    struct UsersTypo {
        char letter1, letter2;
        //        std::string word;
        //        int userId;
    };
    
    bool operator <(const UsersTypo &a, const UsersTypo &b) {
        if (a.letter1 == b.letter1) return a.letter2 < b.letter2;
        return a.letter1 < b.letter1;
    }
    
    struct AbstractTextGenerator {
        virtual std::vector<std::string> getTop(int k) = 0;
        virtual std::vector<std::string> getTopByMistakes(UsersTypo mistake, int k) = 0;
    };
    
    struct AdaptiveTextGenerator : AbstractTextGenerator {
        std::map<UsersTypo, std::vector<std::string>> wordsByTypo;
        // TODO replace with DB ? 
        std::vector<std::string> words;
        AdaptiveTextGenerator(std::string &filename) {
            std::fstream in(filename, std::ios::in);
            std::string corpus_line;
            // TODO replace with DB assign?
            while (std::getline(in, corpus_line)) {
                for (int ind = 1; ind < corpus_line.size(); ind++) {
                    wordsByTypo[{corpus_line[ind - 1], corpus_line[ind]}].push_back(corpus_line);
                }
                words.push_back(corpus_line);
            }
        }
        std::vector<std::string> getTop(int k) override {
            std::vector<std::string> result(k);
            for (int ind = 0; ind < k; ind++) {
                result[ind] = words[rnd() % words.size()];
            }
            return  result;
        }

        std::vector<std::string> getTopByMistakes(UsersTypo mistake, int k) override {
            std::vector<std::string> result(k);
            if (wordsByTypo[mistake].size() < k) {
                std::cerr << "ALERT! not enough words with such a misstake";
            }
            for (int ind =0; ind < k; ind++) {
                result[ind] = wordsByTypo[mistake][rnd() % wordsByTypo[mistake].size()];
            }
            return  result; 
        }
    };
}

#endif//FASTTYPING_GENERATOR_H
