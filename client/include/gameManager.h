#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "gameLogic.h"

#include <QObject>
#include <optional>

class GameManager : public QObject
{
    Q_OBJECT
public:
    virtual ~GameManager() {}
    virtual void key_pressed(QChar button) = 0;
    virtual void backspace_pressed() = 0;
    virtual QString get_buffer() = 0;
    virtual std::optional<QChar> next() = 0;
signals:
    void error_signal();
    void correct_signal();
};

#endif// GAME_MANAGER_H
