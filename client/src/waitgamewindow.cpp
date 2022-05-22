#include "waitgamewindow.h"
#include "ui_waitgamewindow.h"
#include <nlohmann/json.hpp>
#include "windowcontroller.h"
#include "errorHandler.h"
#include "gameContextManager.h"
#include "queryTemplates.h"
#include "responseParse.h"
#include "sonicSocket.h"

WaitGameWindow::WaitGameWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::WaitGameWindow) {
    ui->setupUi(this);
}

WaitGameWindow::~WaitGameWindow() {
    delete ui;
}

void WaitGameWindow::on_ReturnButton_clicked() {
    // TODO
}

void WaitGameWindow::showEvent(QShowEvent *event){
    QWidget::showEvent(event);
    using client::queries::wait_game_query;
    using client::responses::ensure_success;
    using client::responses::error_text;
    using client::web::socket;
    using nlohmann::json;
    QString raw_response = socket().query(wait_game_query());
    qDebug() << "wait game result: " << raw_response;
    json response = json::parse(raw_response.toStdString());

    if (ensure_success(response)) {
        auto &context = ContextManager::get_instance();
        context.set_context_from_create_query(response);
        auto &controller = FastTyping::WindowController::getInstance();
        controller.setActiveWindow("GameWindow");
    }
}
