#include "startgamewindow.h"
#include "ui_startgamewindow.h"
#include "windowcontroller.h"

StartGameWindow::StartGameWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::StartGameWindow) {
    ui->setupUi(this);
}

StartGameWindow::~StartGameWindow() {
    delete ui;
}

void StartGameWindow::on_StartGameButton_clicked() {
    auto &controller = FastTyping::WindowController::getInstance();
    controller.setActiveWindow("GameWindow");
}
