#include "joinwindow.h"
#include "./ui_joinwindow.h"
#include "windowcontroller.h"
#include "sonicSocket.h"
#include "queryTemplates.h"
#include "errorHandler.h"
#include <iostream>

JoinWindow::JoinWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::JoinWindow) {
    ui->setupUi(this);
}

JoinWindow::~JoinWindow() {
    delete ui;
}


void JoinWindow::on_JoinButton_clicked() {
    using client::web::socket;
    using client::queries::join_query;

    bool is_correct_id = true;
    int id =  ui->lineEdit->displayText().toInt(&is_correct_id);
    QString response = socket().query(join_query(id));
    qDebug() << "join result: " << response;
    //error_alert("Error while joining", "Wrong ID");
    auto &controller = FastTyping::WindowController::getInstance();
    controller.setActiveWindow("GameWindow");
}


void JoinWindow::on_ReturnButton_clicked() {
    auto &controller = FastTyping::WindowController::getInstance();
    controller.setActiveWindow("MainWindow");
}
