#include "createwindow.h"
#include "ui_createwindow.h"
#include "sonicSocket.h"
#include "queryTemplates.h"
#include "windowcontroller.h"

CreateWindow::CreateWindow(QWidget *parent) : QMainWindow(parent),
                                              ui(new Ui::CreateWindow) {
    ui->setupUi(this);
    //ui->SessionId->setText(TODO);
}

CreateWindow::~CreateWindow() {
    delete ui;
}

void CreateWindow::on_ReturnButton_clicked() {
    auto &controller = FastTyping::WindowController::getInstance();
    controller.setActiveWindow("MainWindow");
}


void CreateWindow::on_CreateButton_clicked() {
    auto &controller = FastTyping::WindowController::getInstance();
    // TODO get info about game
    using client::web::socket;
    using client::queries::create_game_query;
    QString response = socket().query(create_game_query());
    qDebug() << "create result:" << response;
    controller.setActiveWindow("GameWindow");
}
