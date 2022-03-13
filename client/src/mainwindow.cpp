#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "windowcontroller.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::on_pushButton_clicked() {
    auto &controller = FastTyping::WindowController::getInstance();
    std::cerr << "JOPA";
    controller.setActiveWindow("JoinWindow");
}
