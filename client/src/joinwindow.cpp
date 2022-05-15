#include "joinwindow.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include "./ui_joinwindow.h"
#include "errorHandler.h"
#include "gameContextManager.h"
#include "queryTemplates.h"
#include "responseParse.h"
#include "sonicSocket.h"
#include "windowcontroller.h"

JoinWindow::JoinWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::JoinWindow) {
    ui->setupUi(this);
}

JoinWindow::~JoinWindow() {
    delete ui;
}

// cppcheck-suppress unusedFunction
void JoinWindow::on_JoinButton_clicked() {
    using client::queries::join_query;
    using client::responses::ensure_success;
    using client::responses::error_text;
    using client::web::socket;
    using nlohmann::json;

    bool is_correct_id = true;
    int id = ui->lineEdit->displayText().toInt(&is_correct_id);

    if (!is_correct_id) {
        error_alert("Wrong Id", "Id should be non negative integer");
        return;
    }

    QString raw_response = socket().query(join_query(id));
    qDebug() << "join result: " << raw_response;
    json response = json::parse(raw_response.toStdString());

    if (ensure_success(response)) {
        auto &context = ContextManager::get_instance();
        context.set_context_from_create_query(response);
        auto &controller = FastTyping::WindowController::getInstance();
        controller.setActiveWindow("GameWindow");
    }
}

void JoinWindow::on_ReturnButton_clicked() {
    auto &controller = FastTyping::WindowController::getInstance();
    controller.setActiveWindow("MainWindow");
}
