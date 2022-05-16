#include "gamewindow.h"
#include <iostream>
#include "confirmWindow.h"
#include "gameContextManager.h"
#include "queryTemplates.h"
#include "sonicSocket.h"
#include "ui_gamewindow.h"
#include "windowcontroller.h"

GameWindow::GameWindow(std::vector<GameManager *> managers,
                       GameManager *manager,
                       QWidget *parent)
    : QMainWindow(parent), ui(new Ui::GameWindow), main_manager(manager) {
    for (auto man : managers) {
        QObject::connect(this, &GameWindow::key_pressed, man,
                         &GameManager::key_pressed);
        QObject::connect(this, &GameWindow::backspace_pressed, man,
                         &GameManager::backspace_pressed);
    }

    QObject::connect(manager, &GameManager::correct_signal, this,
                     &GameWindow::correct_slot);
    QObject::connect(manager, &GameManager::error_signal, this,
                     &GameWindow::error_slot);
    QObject::connect(manager, &GameManager::end_signal, this, &GameWindow::end);
    QObject::connect(manager, &GameManager::print_signal, this,
                     &GameWindow::print);

    ui->setupUi(this);
    palette = ui->userText->palette();
    palette.setColor(ui->userText->backgroundRole(), Qt::white);
    palette.setColor(ui->userText->foregroundRole(), Qt::black);
    ui->userText->setAutoFillBackground(true);
    ui->userText->setPalette(palette);
    ui->dictLabel->setAutoFillBackground(true);
    ui->dictLabel->setPalette(palette);
    ui->dictLabel->setText(manager->blob());
    ui->game_id->setText(
        QString::number(ContextManager::get_instance().get_game_id()));
}

GameWindow::~GameWindow() {
    delete ui;
}

void GameWindow::on_ReturnButton_clicked() {
    using client::queries::leave_query;
    using client::web::socket;

    if (confirm("Exit", "Are you really want to exit")) {
        ContextManager::get_instance().reset_context();
        qDebug() << "leave result: " << socket().query(leave_query());
        auto &controller = FastTyping::WindowController::getInstance();
        controller.setActiveWindow("MainWindow");
    }
}

void GameWindow::keyPressEvent(QKeyEvent *event) {
    QString keysCombination = event->text();

    if (keysCombination == "") {
        return;
    }

    if (event->key() == Qt::Key_Backspace) {
        emit backspace_pressed();
    } else {
        emit key_pressed(keysCombination[0]);
    }
}

void GameWindow::error_slot() {
    palette.setColor(ui->userText->backgroundRole(), Qt::red);
    ui->userText->setPalette(palette);
}

void GameWindow::correct_slot() {
    palette.setColor(ui->userText->backgroundRole(), Qt::white);
    ui->userText->setPalette(palette);
}

void GameWindow::end() {
    auto &controller = FastTyping::WindowController::getInstance();
    controller.setActiveWindow("StatWindow");
}

void GameWindow::print(QString const &line) {
    ui->userText->setText(line);
}
