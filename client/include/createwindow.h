#ifndef CREATEWINDOW_H
#define CREATEWINDOW_H

#include <QMainWindow>

namespace Ui {
    class CreateWindow;
}

class CreateWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit CreateWindow(QWidget *parent = nullptr);
    ~CreateWindow();

private:
    Ui::CreateWindow *ui;

private slots:
    void on_ReturnButton_clicked();
    void on_CreateButton_clicked();
};

#endif// CREATEWINDOW_H
