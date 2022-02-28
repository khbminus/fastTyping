#ifndef RUS_H
#define RUS_H

#include <string>
#include <vector>
#include <unordered_map>

class Word {
protected:
    std::string m_infinitive;
    std::vector<std::string> m_forms;
public:
    std::string infinitive() const;
    std::string random_form() const;
    std::vector<std::string> const& forms() const;
};

class RusWord : public Word{
public:
    RusWord(std::string& corpus_line);
    RusWord(RusWord const&) = default;
    RusWord() = default;
};

class RusLoader {
    // Should be replaced with DB connection soon; Not for real use;
public:
    std::unordered_map<std::string, RusWord> infinitive_to_word;
    RusLoader(std::string const& filename);
};

#endif