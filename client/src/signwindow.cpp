#include "signwindow.h"
#include "sonicSocket.h"
#include "queryTemplates.h"
#include "ui_signwindow.h"
#include "windowcontroller.h"

SignWindow::SignWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::SignWindow) {
    ui->setupUi(this);
    ui->Password->setPlaceholderText("password");
    ui->Username->setPlaceholderText("username");
}

SignWindow::~SignWindow() {
    delete ui;
}

void SignWindow::on_SubmitButton_clicked() {
    using client::web::socket;
    using client::queries::sign_in_query;
    using client::queries::greeting_query;

    QString username = ui->Username->displayText();
    [[maybe_unused]]QString password = ui->Username->displayText();
    QString raw_response = socket().query(greeting_query(username));
    qDebug() << "sign in result" << raw_response;
    auto &controller = FastTyping::WindowController::getInstance();
    controller.setActiveWindow("MainWindow");
}


void SignWindow::on_ReturnButton_clicked() {
    auto &controller = FastTyping::WindowController::getInstance();
    controller.setActiveWindow("LoginWindow");
}
