#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "windowcontroller.h"
#include <iostream>

GameWindow::GameWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::GameWindow) {
    ui->setupUi(this);
    palette = ui->userText->palette();
    palette.setColor(ui->userText->backgroundRole(), Qt::white);
    palette.setColor(ui->userText->foregroundRole(), Qt::black);
    ui->userText->setAutoFillBackground(true);
    ui->userText->setPalette(palette);
    ui->dictLabel->setAutoFillBackground(true);
    ui->dictLabel->setPalette(palette);
}


GameWindow::~GameWindow() {
    delete ui;
}

void GameWindow::keyPressEvent(QKeyEvent *event) {
    QString keysCombination = event->text();
    //    qDebug() << keysCombination;

    if (keysCombination == "") {
        // for shift, ctrl, alt
        return;
    }
    if (event->key() == Qt::Key_Backspace) {
        // TODO parse backspace without KOSTYL'
        if (curInput.size() != 0) {
            std::cerr << "deleted\n";
            curInput.chop(1);
            ui->userText->setText(curInput);
            return;
        }
    }

    emit keyPressed(keysCombination[0]);
}

void GameWindow::keyPressed(QChar key) {
    curInput += key;
    ui->userText->setText(curInput);
    ui->userText->setPalette(palette);
}

void GameWindow::setError() {
    palette.setColor(ui->userText->backgroundRole(), Qt::darkRed);
}
void GameWindow::unsetError() {
    palette.setColor(ui->userText->backgroundRole(), Qt::white);
}
