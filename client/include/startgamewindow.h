#ifndef STARTGAMEWINDOW_H
#define STARTGAMEWINDOW_H

#include <QMainWindow>

namespace Ui {
class StartGameWindow;
}

class StartGameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StartGameWindow(QWidget *parent = nullptr);
    ~StartGameWindow();

private slots:
    void on_StartGameButton_clicked();

private:
    Ui::StartGameWindow *ui;
};

#endif // STARTGAMEWINDOW_H
