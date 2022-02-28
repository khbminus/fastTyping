#include "rus.h"
#include <fstream>
#include <iostream>
int main() {
    std::fstream in("paradigms_rus");
    std::fstream out("out.txt", std::ios::out);

    std::string corpus_line;

    std::cout << "start" << std::endl;

    while (std::getline(in, corpus_line)) {
        RusWord word(corpus_line);
        out << word.random_form() << std::endl;
    }

    std::cout << "end" << std::endl;
}