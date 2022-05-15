#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "windowcontroller.h"

LoginWindow::LoginWindow(QWidget *parent) : QMainWindow(parent),
                                            ui(new Ui::LoginWindow) {
    ui->setupUi(this);
}

LoginWindow::~LoginWindow() {
    delete ui;
}

void LoginWindow::on_SignButton_clicked() {
    auto &controller = FastTyping::WindowController::getInstance();
    controller.setActiveWindow("SignWindow");
}


void LoginWindow::on_RegisterButton_clicked() {
    auto &controller = FastTyping::WindowController::getInstance();
    controller.setActiveWindow("RegisterWindow");
}
