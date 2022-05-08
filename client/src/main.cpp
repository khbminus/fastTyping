#include "client/include/createwindow.h"
#include "client/include/gamewindow.h"
#include "client/include/joinwindow.h"
#include "client/include/localManager.h"
#include "client/include/mainwindow.h"
#include "client/include/responseHandler.h"
#include "client/include/sonicSocket.h"
#include "client/include/statwindow.h"
#include "client/include/webManager.h"
#include "windowcontroller.h"

#include <QApplication>
#include <QSharedPointer>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    client::responses::APIHandler response_handler;
    client::web::socket(&response_handler);

    LocalManager manager({"This", "is", "sample", "don't", "judge", "me"});
/*

    WebManager manager({"This", "is", "sample", "don't", "judge", "me"});

    QObject::connect(&response_handler, &client::responses::APIHandler::correct_signal, &manager, &WebManager::correct_slot);
    QObject::connect(&response_handler, &client::responses::APIHandler::error_signal, &manager, &WebManager::error_slot);
    QObject::connect(&response_handler, &client::responses::APIHandler::end_signal, &manager, &WebManager::end_slot);
    QObject::connect(&response_handler, &client::responses::APIHandler::correct_word_signal, &manager, &WebManager::correct_word_slot);
    //*/
    auto &controller = FastTyping::WindowController::getInstance();
    auto mainWindow = QSharedPointer<QMainWindow>(new MainWindow());
    auto joinWindow = QSharedPointer<QMainWindow>(new JoinWindow());
    auto createWindow = QSharedPointer<QMainWindow>(new CreateWindow());
    auto gameWindow = QSharedPointer<QMainWindow>(new GameWindow(&manager));
    auto statWindow = QSharedPointer<QMainWindow>(new StatWindow());
    controller.registerWindow("MainWindow", mainWindow);
    controller.registerWindow("JoinWindow", joinWindow);
    controller.registerWindow("CreateWindow", createWindow);
    controller.registerWindow("GameWindow", gameWindow);
    controller.registerWindow("StatWindow", statWindow);
    controller.setActiveWindow("MainWindow");
    return a.exec();
}
