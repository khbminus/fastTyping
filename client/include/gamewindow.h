#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QChar>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>

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
    QString curInput;
    QPalette palette;


protected:
    void keyPressEvent(QKeyEvent *event);

private slots:
    void keyPressed(QChar key);
};

#endif// GAMEWINDOW_H
