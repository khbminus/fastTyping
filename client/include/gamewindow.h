#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

//#include <QChar>
#include "gameLogic.h"
#include "gameClient.h"
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
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();
    void setError();
    void unsetError();

private:
    Ui::GameWindow *ui;
    QPalette palette;

    std::vector<game::AbstractGameManager*> handlers;
    game::AbstractGameManager* main_handler;
protected:
    void keyPressEvent(QKeyEvent *event);
private slots:
    void keyPressed();
    void on_ReturnButton_clicked();
};

#endif// GAMEWINDOW_H
