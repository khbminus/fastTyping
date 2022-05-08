#ifndef MANAGER_UTILS_H
#define MANAGER_UTILS_H

#include "gameManager.h"
#include <algorithm>
#include <iterator>


class LocalDictionary : public Dictionary {
    std::vector<QString> words;
    std::size_t currentPosition = 0;
    QString whole_line;
public:
    explicit LocalDictionary(std::vector<QString> a_words) : words(std::move(a_words)) {
        QStringList list;
        list.reserve(words.size());
        std::copy(words.begin(), words.end(), std::back_inserter(list));
        whole_line = list.join(" ");
    }

    [[nodiscard]] QString getCurrentWord() const override {
        return words.at(currentPosition);
    }

    bool nextWord() override {
        currentPosition++;
        return currentPosition < words.size();
    }

    QString blob() {
        return whole_line;
    }
};

class LocalInputter : public Inputter {
private:
    QString buffer;

public:
    void addSymbol(QChar button) override {
        buffer += button;
    }
    void deleteSymbol() override {
        if (!buffer.isEmpty()) {
            buffer.chop(1);
        }
    }
    [[nodiscard]] QString getBuffer() const override {
        return buffer;
    }
    void clearBuffer() override {
        buffer.clear();
    }
};


#endif
