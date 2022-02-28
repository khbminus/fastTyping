#include "rus.h"
#include <fstream>
#include <iostream>
int main() {
    std::fstream out("out.txt", std::ios::out);

    std::string corpus_line;

    std::cout << "start" << std::endl;

    RusLoader loader("paradigms_rus");

    for (auto const &[infinitive, word]: loader.infinitive_to_word) {
        out << infinitive << " " << word.random_form() << std::endl;
    }

    std::cout << "end" << std::endl;
}