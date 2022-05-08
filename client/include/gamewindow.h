#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "gameManager.h"
#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <memory>

namespace Ui {
    class GameWindow;
}

class GameWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit GameWindow(GameManager *manager, QWidget *parent = nullptr);
    ~GameWindow();

private:
    Ui::GameWindow *ui;
    QPalette palette;

    //std::vector<game::AbstractGameManager *> handlers;
    //game::AbstractGameManager *main_handler;
    GameManager *main_manager;

protected:
    void keyPressEvent(QKeyEvent *event);
private slots:
    void on_ReturnButton_clicked();
    void error_slot();
    void correct_slot();
    void print(QString const &line);
    void end();
};

#endif// GAMEWINDOW_H
