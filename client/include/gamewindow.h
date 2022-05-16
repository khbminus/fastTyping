#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QVariant>
#include <memory>
#include "gameManager.h"

namespace Ui {
class GameWindow;
}

class GameWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit GameWindow(std::vector<GameManager *> managers,
                        GameManager *manager,
                        QWidget *parent = nullptr);
    ~GameWindow();

private:
    Ui::GameWindow *ui;
    QPalette palette;

    //    std::vector<game::AbstractGameManager *> handlers;
    //    game::AbstractGameManager *main_handler;
    GameManager *main_manager;
signals:
    void press(QVariant key);
    void release(QVariant key);
    // std::vector<game::AbstractGameManager *> handlers;
    // game::AbstractGameManager *main_handler;

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
private slots:
    void on_ReturnButton_clicked();
    void error_slot();
    void correct_slot();
    void print(QString const &line);
    void end();
signals:
    void backspace_pressed();
    void key_pressed(QChar button);
};

#endif  // GAMEWINDOW_H
