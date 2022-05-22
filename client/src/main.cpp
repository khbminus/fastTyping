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
#include "startgamewindow.h"
#include "statwindow.h"
#include "waitgamewindow.h"
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

    // LocalManager manager({"This", "is", "sample", "don't", "judge", "me"});
    /*
        WebManager manager({"This", "is", "sample", "don't", "judge", "me"});

        QObject::connect(&response_handler,
                         &client::responses::APIHandler::correct_signal,
       &manager, &WebManager::correct_slot); QObject::connect(&response_handler,
                         &client::responses::APIHandler::error_signal, &manager,
                         &WebManager::error_slot);
        QObject::connect(&response_handler,
                         &client::responses::APIHandler::end_signal, &manager,
                         &WebManager::end_slot);
        QObject::connect(&response_handler,
                         &client::responses::APIHandler::correct_word_signal,
                         &manager, &WebManager::correct_word_slot);
        //*/
    auto &controller = FastTyping::WindowController::getInstance();

    auto mainWindow = QSharedPointer<QMainWindow>(new MainWindow());
    auto joinWindow = QSharedPointer<QMainWindow>(new JoinWindow());
    auto createWindow = QSharedPointer<QMainWindow>(new CreateGameWindow());
    // auto gameWindow = QSharedPointer<QMainWindow>(new GameWindow(&manager));
    auto statWindow = QSharedPointer<QMainWindow>(new StatWindow());
    auto loginWindow = QSharedPointer<QMainWindow>(new LoginWindow());
    auto signWindow = QSharedPointer<QMainWindow>(new SignWindow());
    auto registerWindow = QSharedPointer<QMainWindow>(new RegisterWindow());
    auto startGameWindow = QSharedPointer<QMainWindow>(new StartGameWindow());
    auto waitGameWindow = QSharedPointer<QMainWindow>(new RegisterWindow());

    controller.registerWindow("LoginWindow", loginWindow);
    controller.registerWindow("SignWindow", signWindow);
    controller.registerWindow("RegisterWindow", registerWindow);
    controller.registerWindow("StartGameWindow", startGameWindow);
    controller.registerWindow("WaitGameWindow", waitGameWindow);

    controller.registerWindow("MainWindow", mainWindow);
    controller.registerWindow("JoinWindow", joinWindow);
    controller.registerWindow("CreateWindow", createWindow);
    // controller.registerWindow("GameWindow", gameWindow);
    controller.registerWindow("StatWindow", statWindow);
    controller.setActiveWindow("LoginWindow");
    return a.exec();
}
