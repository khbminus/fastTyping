#ifndef LOCAL_MANAGER_H
#define LOCAL_MANAGER_H

#include "gameManager.h"

#include <QObject>
#include <optional>
#include <vector>

class LocalDictionary : public Dictionary {
    std::vector<QString> words;
    std::size_t currentPosition = 0;
public:
    explicit LocalDictionary(std::vector<QString> a_words) : words(std::move(a_words)) {}

    [[nodiscard]] QString getCurrentWord() const override {
        return words.at(currentPosition);
    }

    bool nextWord() override {
        currentPosition++;
        return currentPosition < words.size();
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

class LocalManager final : public GameManager {
    LocalInputter inputter;
    LocalDictionary dictionary;
    void check_prefix();
public:
    LocalManager(std::vector<QString> words);
    void key_pressed(QChar button) override;
    void backspace_pressed() override;
    QString get_buffer() override;
    std::optional<QChar> next() override;

};

#endif// GAME_MANAGER_H
