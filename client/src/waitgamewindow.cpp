#include "waitgamewindow.h"
#include <nlohmann/json.hpp>
#include "errorHandler.h"
#include "responseHandler.h"
#include "gameContextManager.h"
#include "queryTemplates.h"
#include "responseParse.h"
#include "sonicSocket.h"
#include "ui_waitgamewindow.h"
#include "windowcontroller.h"

WaitGameWindow::WaitGameWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::WaitGameWindow) {
    ui->setupUi(this);

    QObject::connect(&client::responses::handler(), &client::responses::APIHandler::gameWaited, this,
                     &WaitGameWindow::gameWaited);
}

WaitGameWindow::~WaitGameWindow() {
    delete ui;
}

void WaitGameWindow::on_ReturnButton_clicked() {
    // TODO
}


void WaitGameWindow::gameWaited() {
    auto &controller = FastTyping::WindowController::getInstance();
    controller.setActiveWindow("GameWindow");
}

void WaitGameWindow::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    using client::queries::wait_game_query;
    using client::responses::ensure_success;
    using client::responses::error_text;
    using client::web::socket;
    using nlohmann::json;
    socket().send(wait_game_query());
    qDebug() << "wait game result: ";
}
