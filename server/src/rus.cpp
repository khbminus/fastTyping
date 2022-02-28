#include "rus.h"
#include <random>
#include <ctime>
#include <iostream>
#include <sstream>
#include <algorithm>

std::string Word::infinitive() const {
    return m_infinitive;
}

std::string Word::random_form() const {
    static std::mt19937 rnd(std::time(0));
    return m_forms[rnd() % m_forms.size()];
}

std::vector<std::string> const& Word::forms() const {
    return m_forms;
}

RusWord::RusWord(std::string &corpus_line) {
    std::stringstream line(corpus_line + ",");
    std::getline(line, m_infinitive, '#');
    std::string form;
    while (std::getline(line, form, ',')) {
        form.erase(std::remove(form.begin(), form.end(), '\''), form.end());
        m_forms.push_back(form);
    }
}

