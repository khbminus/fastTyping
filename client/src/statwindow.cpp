#include "statwindow.h"
#include "ui_statwindow.h"
#include "windowcontroller.h"

StatWindow::StatWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::StatWindow) {
    ui->setupUi(this);
}

StatWindow::~StatWindow() {
    delete ui;
}

void StatWindow::on_ReturnButton_clicked() {
    auto &controller = FastTyping::WindowController::getInstance();
    controller.setActiveWindow("MainWindow");
}
