#include <QApplication>
#include <QSharedPointer>
#include "createwindow.h"
#include "gamewindow.h"
#include "joinwindow.h"
#include "keyboard.h"
#include "localManager.h"
#include "loginwindow.h"
#include "mainwindow.h"
#include "registerwindow.h"
#include "responseHandler.h"
#include "signwindow.h"
#include "sonicSocket.h"
#include "statwindow.h"
#include "webManager.h"
#include "windowcontroller.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    auto &keyboard = FastTyping::Keyboard::KeyboardModel::getInstance();
    keyboard.addPath("qwerty.json");
    keyboard.addPath("test.json");
    keyboard.setCurrentLayout(0);

    client::responses::APIHandler &response_handler =
        client::responses::handler();
    client::web::socket(&response_handler);
    auto &controller = FastTyping::WindowController::getInstance();

    auto mainWindow = QSharedPointer<QMainWindow>(new MainWindow());
    auto joinWindow = QSharedPointer<QMainWindow>(new JoinWindow());
    auto createWindow = QSharedPointer<QMainWindow>(new CreateGameWindow());
    // auto gameWindow = QSharedPointer<QMainWindow>(new GameWindow(&manager));
    auto statWindow = QSharedPointer<QMainWindow>(new StatWindow());
    auto loginWindow = QSharedPointer<QMainWindow>(new LoginWindow());
    auto signWindow = QSharedPointer<QMainWindow>(new SignWindow());
    auto registerWindow = QSharedPointer<QMainWindow>(new RegisterWindow());

    controller.registerWindow("LoginWindow", loginWindow);
    controller.registerWindow("SignWindow", signWindow);
    controller.registerWindow("RegisterWindow", registerWindow);

    controller.registerWindow("MainWindow", mainWindow);
    controller.registerWindow("JoinWindow", joinWindow);
    controller.registerWindow("CreateWindow", createWindow);
    controller.registerWindow("StatWindow", statWindow);
    controller.setActiveWindow("LoginWindow");
    return a.exec();
}
