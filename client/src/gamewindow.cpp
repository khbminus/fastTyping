#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "windowcontroller.h"
#include <iostream>

GameWindow::GameWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::GameWindow), game(new game::RaceGame) {
    ui->setupUi(this);
    palette = ui->userText->palette();
    palette.setColor(ui->userText->backgroundRole(), Qt::white);
    palette.setColor(ui->userText->foregroundRole(), Qt::black);
    ui->userText->setAutoFillBackground(true);
    ui->userText->setPalette(palette);
    ui->dictLabel->setAutoFillBackground(true);
    ui->dictLabel->setPalette(palette);
    ui->dictLabel->setText("This is sample don't judge me");
}


GameWindow::~GameWindow() {
    delete ui;
    delete game;
}

void GameWindow::on_ReturnButton_clicked() {
    auto &controller = FastTyping::WindowController::getInstance();
    controller.setActiveWindow("MainWindow");
}

void GameWindow::keyPressEvent(QKeyEvent *event) {
    QString keysCombination = event->text();
    //    qDebug() << keysCombination;

    if (keysCombination == "") {
        // for shift, ctrl, alt
        return;
    }
    if (event->key() == Qt::Key_Backspace) {
        game->backspacePressed();
        emit keyPressed();
        return;
    }
    game->keyPressed(keysCombination[0]);

    emit keyPressed();
}

void GameWindow::keyPressed() {
    ui->userText->setText(game->getBuffer());
    if (game->getErrorStatus())
        setError();
    else
        unsetError();
    ui->userText->setPalette(palette);
    if (game->isEnded()) {
        auto &controller = FastTyping::WindowController::getInstance();
        controller.setActiveWindow("StatWindow");
    }
}

void GameWindow::setError() {
    palette.setColor(ui->userText->backgroundRole(), Qt::darkRed);
}

void GameWindow::unsetError() {
    palette.setColor(ui->userText->backgroundRole(), Qt::white);
}
