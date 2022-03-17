#ifndef GAMELOGIC_H_
#define GAMELOGIC_H_
#include <QChar>
#include <QString>
#include <memory>
#include <string>
#include <vector>

namespace game {
    struct AbstractDictionary {
        [[nodiscard]] virtual QString getCurrentWord() const = 0;
        virtual bool nextWord() = 0;
    };

    struct AbstractOutputter {
        virtual void printSymbol(char) = 0;
        virtual void highlightError(std::size_t position) = 0;
        virtual void highlightPositions(std::size_t howMany) = 0;
    };

    struct AbstractInputter {
        virtual void addSymbol(QChar) = 0;
        virtual void deleteSymbol() = 0;
        [[nodiscard]] virtual QString getBuffer() const = 0;
        virtual void clearBuffer() = 0;
        //        virtual bool endOfInput() const = 0;
    };

    struct AbstractParser {
        virtual bool isCorrect(const std::string &inputWord, const std::string &dictionaryWord) const;
    };

    struct AbstractDictionaryLoader {
        virtual std::unique_ptr<AbstractDictionary> loadDictionary(const std::string &name) = 0;
    };

    struct ConstDictionary : AbstractDictionary {
        std::vector<QString> words;
        std::size_t currentPosition = 0;
        explicit ConstDictionary(std::vector<QString> words_) : words(std::move(words_)) {}

        [[nodiscard]] QString getCurrentWord() const override {
            return words.at(currentPosition);
        }

        bool nextWord() override {
            currentPosition++;
            return currentPosition < words.size();
        }
    };

    struct Inputter : AbstractInputter {
    private:
        QString buffer;
        //        explicit Inputter(std::string d) : data(std::move(d)) {}
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

    struct AbstractGameManager {
        virtual void keyPressed(QChar key) = 0;
        virtual void backspacePressed() = 0;
        [[nodiscard]] virtual bool getErrorStatus() const = 0;
        [[nodiscard]] virtual bool isEnded() const = 0;
        [[nodiscard]] virtual QString getBuffer() const = 0;
        virtual ~AbstractGameManager() {}
    };

    struct RaceGame : AbstractGameManager{
    private:
        ConstDictionary dictionary;
        Inputter inputter;
        bool errorState;
        bool endOfGame;

        void checkPrefix(QString a, QString b);

    public:
        RaceGame();
        void keyPressed(QChar button) override;
        void backspacePressed() override;
        [[nodiscard]] bool getErrorStatus() const  override;
        [[nodiscard]] bool isEnded() const  override;
        [[nodiscard]] QString getBuffer() const  override;
    };

}// namespace game


#endif// GEMELOGIC_H_
