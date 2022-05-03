#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "windowcontroller.h"
#include "sonicSocket.h"
#include "queryTemplates.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow) {
    using client::web::socket;
    using client::queries::greeting_query;
    ui->setupUi(this);
    std::string answer = socket().query(greeting_query("Aboba")).toStdString();
    std::cout << "init << " << answer << ")))" << std::endl;
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_JoinButton_clicked() {
    auto &controller = FastTyping::WindowController::getInstance();
    controller.setActiveWindow("JoinWindow");
}


void MainWindow::on_CreateButton_clicked() {
    auto &controller = FastTyping::WindowController::getInstance();
    controller.setActiveWindow("CreateWindow");
}
