#include "gamewindow.h"
#include <iostream>
#include "queryTemplates.h"
#include "sonicSocket.h"
#include "ui_gamewindow.h"
#include "windowcontroller.h"
#include <QQuickItem>
#include <iostream>
#include <qqml.h>

GameWindow::GameWindow(GameManager *manager, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::GameWindow), main_manager(manager) {
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
    ui->dictLabel->setText("This is sample don't judge me");
    connect(this, SIGNAL(press(QVariant)), ui->quickWidget->rootObject(), SLOT(pressKey(QVariant)));
    connect(this, SIGNAL(release(QVariant)), ui->quickWidget->rootObject(), SLOT(releaseKey(QVariant)));
    ui->dictLabel->setText(manager->blob());
}

GameWindow::~GameWindow() {
    delete ui;
}

void GameWindow::on_ReturnButton_clicked() {
    auto &controller = FastTyping::WindowController::getInstance();
    controller.setActiveWindow("MainWindow");
}

// cppcheck-suppress unusedFunction
void GameWindow::keyPressEvent(QKeyEvent *event) {
    QString keysCombination = event->text();

    emit press(event->key());

    if (keysCombination == "") {
        return;
    }

    if (event->key() == Qt::Key_Backspace) {
        main_manager->backspace_pressed();
    } else {
        main_manager->key_pressed(keysCombination[0]);
    }
}

// cppcheck-suppress unusedFunction
void GameWindow::keyReleaseEvent(QKeyEvent *event) {
    emit release(event->key());
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
