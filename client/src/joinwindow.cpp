#include "joinwindow.h"
#include "./ui_joinwindow.h"
#include "windowcontroller.h"
#include <iostream>

JoinWindow::JoinWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::JoinWindow) {
    ui->setupUi(this);
}

JoinWindow::~JoinWindow() {
    delete ui;
}


void JoinWindow::on_JoinButton_clicked() {
    auto &controller = FastTyping::WindowController::getInstance();
    controller.setActiveWindow("GameWindow");
}


void JoinWindow::on_ReturnButton_clicked() {
    auto &controller = FastTyping::WindowController::getInstance();
    controller.setActiveWindow("MainWindow");
}