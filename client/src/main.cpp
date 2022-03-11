#include "client/include/mainwindow.h"
#include "client/include/joinwindow.h"
#include <QApplication>
#include <QSharedPointer>
#include "windowcontroller.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    auto& controller = FastTyping::WindowController::getInstance();
    auto mainWindow = QSharedPointer<QMainWindow>(new MainWindow());
    auto joinWindow = QSharedPointer<QMainWindow>(new JoinWindow());
    controller.registerWindow("MainWindow", mainWindow);
    controller.registerWindow("JoinWindow", joinWindow);
    controller.setActiveWindow("MainWindow");
    return a.exec();
}
