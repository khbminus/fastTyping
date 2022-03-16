#ifndef STATWINDOW_H
#define STATWINDOW_H

#include <QMainWindow>

namespace Ui {
class StatWindow;
}

class StatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StatWindow(QWidget *parent = nullptr);
    ~StatWindow();

private slots:
    void on_ReturnButton_clicked();

private:
    Ui::StatWindow *ui;
};

#endif // STATWINDOW_H
