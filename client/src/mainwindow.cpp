#include "mainwindow.h"
#include <iostream>
#include "./ui_mainwindow.h"
#include "queryTemplates.h"
#include "sonicSocket.h"
#include "windowcontroller.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    using client::queries::greeting_query;
    using client::queries::leave_query;
    using client::web::socket;

    ui->setupUi(this);
    QString answer = socket().query(greeting_query("Aboba"));
    // socket().query(leave_query());
    qDebug() << "login result: " << answer;
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