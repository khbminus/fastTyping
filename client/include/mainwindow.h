#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QChar>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
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
    QLabel *userText;
    QGridLayout *layout;

    QString curInput;

protected:
    void keyPressEvent(QKeyEvent *event);

private slots:
    void keyPressed(QChar key);
    static QLabel *createLabel(const QString &text);
};
#endif// MAINWINDOW_H
