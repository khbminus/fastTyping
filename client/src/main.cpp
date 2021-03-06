#include <QApplication>
#include <QSharedPointer>
#include "createwindow.h"
#include "gamewindow.h"
#include "joinwindow.h"
#include "keyboard.h"
#include "localManager.h"
#include "loginwindow.h"
#include "mainwindow.h"
#include "profileWindow.h"
#include "registerwindow.h"
#include "responseHandler.h"
#include "signwindow.h"
#include "sonicSocket.h"
#include "startgamewindow.h"
#include "statwindow.h"
#include "textScreen.h"
#include "waitgamewindow.h"
#include "webManager.h"
#include "windowcontroller.h"

int main(int argc, char *argv[]) {
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
    QQuickView::setGraphicsApi(QSGRendererInterface::OpenGL);

    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("icon.png"));
    QApplication::setAttribute(Qt::AA_ForceRasterWidgets, false);
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
    auto &keyboard = FastTyping::Keyboard::KeyboardModel::getInstance();
    keyboard.addPath("qwerty.json");
    keyboard.addPath("dvorak.json");
    keyboard.addPath("colemak.json");
    //    keyboard.addPath("test.json");
    keyboard.setCurrentLayout(0);

    client::responses::APIHandler &response_handler =
        client::responses::handler();
    client::web::socket(&response_handler);
    auto &controller = FastTyping::WindowController::getInstance();

    auto mainWindow = new MainWindow;
    auto joinWindow = new JoinWindow;
    auto createWindow = new CreateGameWindow;
    //        auto gameWindow = new GameWindow(&manager));
    //        auto statWindow = new StatWindow;
    auto loginWindow = new LoginWindow;
    auto signWindow = new SignWindow;
    auto registerWindow = new RegisterWindow;
    auto startGameWindow = new StartGameWindow;
    auto waitGameWindow = new WaitGameWindow;

    controller.registerWindow("LoginWindow", loginWindow);
    controller.registerWindow("SignWindow", signWindow);
    controller.registerWindow("RegisterWindow", registerWindow);
    controller.registerWindow("StartGameWindow", startGameWindow);
    controller.registerWindow("WaitGameWindow", waitGameWindow);

    controller.registerWindow("MainWindow", mainWindow);
    controller.registerWindow("JoinWindow", joinWindow);
    controller.registerWindow("CreateWindow", createWindow);
    // controller.registerWindow("GameWindow", gameWindow);
    //    controller.registerWindow("StatWindow", statWindow);
    controller.setActiveWindow("LoginWindow");
    return a.exec();
}
