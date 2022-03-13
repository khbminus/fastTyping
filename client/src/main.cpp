#include "client/include/joinwindow.h"
#include "client/include/mainwindow.h"
#include "client/include/gamewindow.h"

#include "windowcontroller.h"
#include <QApplication>
#include <QSharedPointer>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    auto &controller = FastTyping::WindowController::getInstance();
    auto mainWindow = QSharedPointer<QMainWindow>(new MainWindow());
    auto joinWindow = QSharedPointer<QMainWindow>(new JoinWindow());
    auto gameWindow = QSharedPointer<QMainWindow>(new GameWindow());

    controller.registerWindow("MainWindow", mainWindow);
    controller.registerWindow("JoinWindow", joinWindow);
    controller.registerWindow("GameWindow", gameWindow);
    controller.setActiveWindow("MainWindow");
    return a.exec();
}
