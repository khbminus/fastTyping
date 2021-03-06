#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <QObject>
#include <optional>

struct Dictionary {
    [[nodiscard]] virtual QString getCurrentWord() const = 0;
    virtual bool nextWord() = 0;
    virtual bool previousWord() = 0;
};

struct Inputter {
    virtual bool addSymbol(QChar) = 0;
    virtual void deleteSymbol() = 0;
    [[nodiscard]] virtual QString getBuffer() const = 0;
    virtual void clearBuffer() = 0;
};

class GameManager : public QObject {
    Q_OBJECT
public:
    virtual ~GameManager() {}
    virtual QString get_buffer() = 0;
    virtual QVariant next() = 0;
    virtual QString blob() = 0;
public slots:
    virtual void key_pressed(QChar button) = 0;
    virtual void backspace_pressed() = 0;
signals:
    void error_signal();
    void correct_signal();
    void errorOnPositionSignal(int position);
    void correctOnPositionSignal(int position);
    void end_signal();
    void print_signal(QString const &line, int);
};

#endif  // GAME_MANAGER_H
