#include "registerwindow.h"
#include "ui_registerwindow.h"
#include "windowcontroller.h"
#include <iostream>


RegisterWindow::RegisterWindow(QWidget *parent) : QMainWindow(parent),
                                                  ui(new Ui::RegisterWindow) {
    ui->setupUi(this);
}

RegisterWindow::~RegisterWindow() {
    delete ui;
}

void RegisterWindow::on_SubmitButton_clicked() {
    QString name = ui->Username->text();
    QString password = ui->Password->text();
    auto &controller = FastTyping::WindowController::getInstance();
    controller.setActiveWindow("MainWindow");
}


void RegisterWindow::on_ReturnButton_clicked() {
    auto &controller = FastTyping::WindowController::getInstance();
    controller.setActiveWindow("LoginWindow");
}
