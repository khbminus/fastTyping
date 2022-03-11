#include "client/include/mainwindow.h"
#include "windowcontroller.h"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // FIXME: fix this bullshit, we need better code
    auto widget = new QWidget(this);
    setCentralWidget(widget);
    auto layout = new QGridLayout();
    auto button = new QPushButton("Join Game", widget);
    connect(button, SIGNAL(pressed()), this, SLOT(onJoinClick()));
    layout->addWidget(button, 0, 0);
    widget->setLayout(layout);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onJoinClick() {
    auto& controller = FastTyping::WindowController::getInstance();
    controller.setActiveWindow("JoinWindow");
}
