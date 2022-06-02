#include "mainwindow.h"
#include <iostream>
#include "./ui_mainwindow.h"
#include "keyboard.h"
#include "queryTemplates.h"
#include "sonicSocket.h"
#include "windowcontroller.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    using client::queries::greeting_query;
    using client::queries::leave_query;
    using client::web::socket;

    ui->setupUi(this);
    // QString answer = socket().query(greeting_query("Aboba"));
    // socket().query(leave_query());
    // qDebug() << "login result: " << answer;

    ui->layoutView->setModel(
        FastTyping::Keyboard::KeyboardModel::getInstance().tableModel());
    ui->layoutView->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch);
    ui->layoutView->horizontalHeader()->setSectionsClickable(false);
    connect(
        ui->layoutView, &QTableView::doubleClicked,
        [&](const QModelIndex &idx) {
            FastTyping::Keyboard::KeyboardModel::getInstance().setCurrentLayout(
                idx.row());
            qDebug() << "changed layout to" << idx.row();
        });
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

void MainWindow::on_ProfileButton_clicked() {
    auto &controller = FastTyping::WindowController::getInstance();
    controller.setActiveWindow("ProfileWindow");
}
