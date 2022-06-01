#include "startgamewindow.h"
#include <nlohmann/json.hpp>
#include "errorHandler.h"
#include "gameContextManager.h"
#include "queryTemplates.h"
#include "responseParse.h"
#include "sonicSocket.h"
#include "ui_startgamewindow.h"
#include "windowcontroller.h"

StartGameWindow::StartGameWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::StartGameWindow) {
    ui->setupUi(this);
}

StartGameWindow::~StartGameWindow() {
    delete ui;
}

void StartGameWindow::showEvent(QShowEvent *event) {
    ui->game_id->setText(QString::fromStdString(
        std::to_string(ContextManager::get_instance().get_game_id())));
}

void StartGameWindow::on_StartGameButton_clicked() {
    using client::queries::start_query;
    using client::responses::ensure_success;
    using client::responses::error_text;
    using client::web::socket;
    using nlohmann::json;
    QString raw_response = socket().query(start_query());
    qDebug() << "start result: " << raw_response;
    json response = json::parse(raw_response.toStdString());
    if (ensure_success(response)) {
        auto &controller = FastTyping::WindowController::getInstance();
        controller.setActiveWindow("GameWindow");
    }
}
