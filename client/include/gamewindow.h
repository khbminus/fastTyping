#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

//#include <QChar>
#include "gameClient.h"
#include "gameLogic.h"
#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QVariant>
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

    std::vector<game::AbstractGameManager *> handlers;
    game::AbstractGameManager *main_handler;
signals:
    void press(QVariant key);
    void release(QVariant key);

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
private slots:
    void keyPressed();
    void on_ReturnButton_clicked();
};

#endif// GAMEWINDOW_H
