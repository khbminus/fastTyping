#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QMainWindow>
#include <QObject>
#include <QUrl>
#include <QtCore/QUuid>
#include <QtQuick/QtQuick>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_JoinButton_clicked();
    void on_CreateButton_clicked();
    void on_ProfileButton_clicked();
    void on_TopButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif  // MAINWINDOW_H
