#include "registerwindow.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include "queryTemplates.h"
#include "responseParse.h"
#include "sonicSocket.h"
#include "ui_registerwindow.h"
#include "windowcontroller.h"

using nlohmann::json;

RegisterWindow::RegisterWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::RegisterWindow) {
    ui->setupUi(this);
    ui->Password->setPlaceholderText("password");
    ui->Username->setPlaceholderText("username");
}

RegisterWindow::~RegisterWindow() {
    delete ui;
}

void RegisterWindow::on_SubmitButton_clicked() {
    using client::queries::greeting_query;
    using client::queries::sign_on_query;
    using client::responses::ensure_success;
    using client::web::socket;

    QString username = ui->Username->displayText();
    QString password = ui->Password->displayText();
    QString raw_response = socket().query(sign_on_query(username, password));
    qDebug() << "sign in result" << raw_response;
    json response = json::parse(raw_response.toStdString());

    if (ensure_success(response)) {
        auto &controller = FastTyping::WindowController::getInstance();
        controller.setActiveWindow("MainWindow");
    }
}

void RegisterWindow::on_ReturnButton_clicked() {
    auto &controller = FastTyping::WindowController::getInstance();
    controller.setActiveWindow("LoginWindow");
}
