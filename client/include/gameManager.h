#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <QObject>
#include <optional>

struct Dictionary {
    [[nodiscard]] virtual QString getCurrentWord() const = 0;
    virtual bool nextWord() = 0;
};

struct Inputter {
    virtual void addSymbol(QChar) = 0;
    virtual void deleteSymbol() = 0;
    [[nodiscard]] virtual QString getBuffer() const = 0;
    virtual void clearBuffer() = 0;
};

class GameManager : public QObject {
    Q_OBJECT
public:
    virtual ~GameManager() {}
    virtual void key_pressed(QChar button) = 0;
    virtual void backspace_pressed() = 0;
    virtual QString get_buffer() = 0;
    virtual std::optional<QChar> next() = 0;
    virtual QString blob() = 0;
signals:
    void error_signal();
    void correct_signal();
    void end_signal();
    void print_signal(QString const& line);
};

#endif// GAME_MANAGER_H
