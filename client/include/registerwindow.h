#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QMainWindow>

namespace Ui {
class RegisterWindow;
}

class RegisterWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit RegisterWindow(QWidget *parent = nullptr);
    ~RegisterWindow();

private slots:
    void on_SubmitButton_clicked();

    void on_ReturnButton_clicked();

private:
    Ui::RegisterWindow *ui;
};

#endif  // REGISTERWINDOW_H
