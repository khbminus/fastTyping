#ifndef RUS_H
#define RUS_H

#include <string>
#include <vector>

class Word {
protected:
    std::string m_infinitive;
    std::vector<std::string> m_forms;

public:
    std::string infinitive() const;
    std::string random_form() const;
    std::vector<std::string> const &forms() const;
};

class RusWord : public Word {
public:
    RusWord(std::string &corpus_line);
};

#endif