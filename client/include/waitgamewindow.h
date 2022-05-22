#ifndef WAITGAMEWINDOW_H
#define WAITGAMEWINDOW_H

#include <QMainWindow>

namespace Ui {
class WaitGameWindow;
}

class WaitGameWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit WaitGameWindow(QWidget *parent = nullptr);
    ~WaitGameWindow();

private slots:
    void on_ReturnButton_clicked();

private:
    Ui::WaitGameWindow *ui;
};

#endif  // WAITGAMEWINDOW_H
