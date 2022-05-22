#ifndef CreateGameWindow_H
#define CreateGameWindow_H

#include <QMainWindow>

namespace Ui {
class CreateWindow;
}

class CreateGameWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit CreateGameWindow(QWidget *parent = nullptr);
    ~CreateGameWindow();

private:
    Ui::CreateWindow *ui;
private slots:
    void on_ReturnButton_clicked();
    void on_CreateButton_clicked();
    void showEvent(QShowEvent *event);
};

#endif  // CreateGameWindow_H
