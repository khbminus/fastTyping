#ifndef MANAGER_UTILS_H
#define MANAGER_UTILS_H

#include <QDebug>
#include <algorithm>
#include <iterator>
#include "gameManager.h"

class LocalDictionary : public Dictionary {
    std::vector<QString> words;
    std::size_t currentPosition = 0;
    QString whole_line;

public:
    explicit LocalDictionary(std::vector<QString> a_words)
        : words(std::move(a_words)) {
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
    bool previousWord() override {
        if (currentPosition == 0)
            return false;
        currentPosition--;
        return true;
    }

    QString blob() {
        return whole_line;
    }

    [[nodiscard]] int getCompletedSize() const {
        return currentPosition +
               std::accumulate(
                   words.begin(), words.begin() + currentPosition, 0,
                   [&](int a, const QString &b) { return a + b.size(); });
    }
    [[nodiscard]] QString getCompletedLine() const {
        QString ans = std::accumulate(
            words.begin(), words.begin() + currentPosition, QString(),
            [&](const QString &a, const QString &b) { return a + " " + b; });
        return ans.trimmed();
    }
    [[nodiscard]] bool isEnded() const {
        return currentPosition == words.size();
    }
};

class LocalInputter : public Inputter {
private:
    QString buffer;
    std::size_t maxBufferSize = 0;

public:
    bool addSymbol(QChar button) override {
        if (buffer.size() + 1 <= maxBufferSize) {
            buffer += button;
            return true;
        }
        return false;
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

    void setBufferMaxSize(std::size_t maxSize) {
        maxBufferSize = maxSize;
    }
};

class LocalSoloInputter : public Inputter {
private:
    std::vector<QString> buffers;
    std::vector<std::size_t> maxBufSz;

public:
    bool addSymbol(QChar button) override {
        if (buffers.back().size() + 1 <= maxBufSz.back()) {
            buffers.back() += button;
            return true;
        }
        return false;
    }

    void deleteSymbol() override {
        if (!buffers.back().isEmpty()) {
            buffers.back().chop(1);
        } else {
            buffers.pop_back();
            maxBufSz.pop_back();
            if (!buffers.back().isEmpty()) {
                buffers.back().chop(1);
            }
        }
    }
    [[nodiscard]] QString getBuffer() const override {
        return buffers.back();
    }
    void clearBuffer() override {
        buffers.clear();
        maxBufSz.clear();
    }

    void setBufferMaxSize(std::size_t maxSize) {
        maxBufSz.push_back(maxSize);
        buffers.push_back("");
    }

    std::size_t getBuffersCnt() {
        return buffers.size();
    }
};

#endif
