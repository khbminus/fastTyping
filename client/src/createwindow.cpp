#include "createwindow.h"
#include <nlohmann/json.hpp>
#include "errorHandler.h"
#include "queryTemplates.h"
#include "responseParse.h"
#include "sonicSocket.h"
#include "ui_createwindow.h"
#include "windowcontroller.h"

CreateWindow::CreateWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::CreateWindow) {
    ui->setupUi(this);
    // ui->SessionId->setText(TODO);
}

CreateWindow::~CreateWindow() {
    delete ui;
}

void CreateWindow::on_ReturnButton_clicked() {
    auto &controller = FastTyping::WindowController::getInstance();
    controller.setActiveWindow("MainWindow");
}

void CreateWindow::on_CreateButton_clicked() {
    using client::responses::error_text;
    using client::responses::is_success;
    using nlohmann::json;

    auto &controller = FastTyping::WindowController::getInstance();
    // TODO get info about game
    using client::queries::create_game_query;
    using client::web::socket;
    QString raw_response = socket().query(create_game_query());
    qDebug() << "create result:" << raw_response;
    json response = json::parse(raw_response.toStdString());
    if (is_success(response)) {
        controller.setActiveWindow("GameWindow");
    } else {
        error_alert("Can't join game", error_text(response));
    }
}
