#include "signwindow.h"
#include <nlohmann/json.hpp>
#include "gameContextManager.h"
#include "queryTemplates.h"
#include "responseParse.h"
#include "sonicSocket.h"
#include "ui_signwindow.h"
#include "windowcontroller.h"

using nlohmann::json;

SignWindow::SignWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SignWindow) {
    ui->setupUi(this);
    ui->Password->setPlaceholderText("password");
    ui->Username->setPlaceholderText("username");
}

SignWindow::~SignWindow() {
    delete ui;
}

void SignWindow::on_SubmitButton_clicked() {
    using client::queries::greeting_query;
    using client::queries::sign_in_query;
    using client::responses::ensure_success;
    using client::web::socket;

    QString username = ui->Username->displayText();
    QString password = ui->Password->displayText();
    // QString raw_response = socket().query(greeting_query(username));
    // qDebug() << "greeting result" << raw_response;
    QString raw_response = socket().query(sign_in_query(username, password));
    qDebug() << "sign in result" << raw_response;
    json response = json::parse(raw_response.toStdString());
    if (ensure_success(response)) {
        ContextManager::get_instance().set_user_id(
            response["body"]["id"].get<int>());
        auto &controller = FastTyping::WindowController::getInstance();
        controller.setActiveWindow("MainWindow");
    }
}

void SignWindow::on_ReturnButton_clicked() {
    auto &controller = FastTyping::WindowController::getInstance();
    controller.setActiveWindow("LoginWindow");
}
