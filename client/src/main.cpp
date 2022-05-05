#include "include/createwindow.h"
#include "include/gamewindow.h"
#include "include/joinwindow.h"
#include "include/mainwindow.h"
#include "include/statwindow.h"


#include "windowcontroller.h"
#include <QApplication>
#include <QSharedPointer>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    auto &controller = FastTyping::WindowController::getInstance();
    auto mainWindow = QSharedPointer<QMainWindow>(new MainWindow());
    auto joinWindow = QSharedPointer<QMainWindow>(new JoinWindow());
    auto createWindow = QSharedPointer<QMainWindow>(new CreateWindow());
    auto gameWindow = QSharedPointer<QMainWindow>(new GameWindow());
    auto statWindow = QSharedPointer<QMainWindow>(new StatWindow());


    controller.registerWindow("MainWindow", mainWindow);
    controller.registerWindow("JoinWindow", joinWindow);
    controller.registerWindow("CreateWindow", createWindow);
    controller.registerWindow("GameWindow", gameWindow);
    controller.registerWindow("StatWindow", statWindow);
    controller.setActiveWindow("MainWindow");
    return a.exec();
}
