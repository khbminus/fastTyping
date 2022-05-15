#ifndef SIGNWINDOW_H
#define SIGNWINDOW_H

#include <QMainWindow>

namespace Ui {
    class SignWindow;
}

class SignWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit SignWindow(QWidget *parent = nullptr);
    ~SignWindow();

private slots:
    void on_SubmitButton_clicked();

    void on_ReturnButton_clicked();

private:
    Ui::SignWindow *ui;
};

#endif// SIGNWINDOW_H
