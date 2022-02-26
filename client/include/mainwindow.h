#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QChar>
#include <QKeyEvent>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private:
    Ui::MainWindow *ui;

protected:
    void keyPressEvent(QKeyEvent *event);

private slots:
    void keyPressed(QChar key);
};
#endif// MAINWINDOW_H
