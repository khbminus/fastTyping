#include "client/include/createwindow.h"
#include "client/include/gamewindow.h"
#include "client/include/joinwindow.h"
#include "client/include/mainwindow.h"
#include "client/include/statwindow.h"
#include "client/include/sonicSocket.h"
#include "windowcontroller.h"

#include <QApplication>
#include <QSharedPointer>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    client::web::TestHandler aboba;
    client::web::socket(&aboba);


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
