#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "windowcontroller.h"
#include <QQuickItem>
#include <iostream>
#include <qqml.h>

GameWindow::GameWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::GameWindow),
                                          handlers{new game::RaceGame, new game::client::GameClient("aboba")},
                                          main_handler(handlers[0]) {
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
}


GameWindow::~GameWindow() {
    delete ui;
    for (auto handler: handlers) {
        delete handler;
    }
}

void GameWindow::on_ReturnButton_clicked() {
    auto &controller = FastTyping::WindowController::getInstance();
    controller.setActiveWindow("MainWindow");
}

void GameWindow::keyPressEvent(QKeyEvent *event) {
    QString keysCombination = event->text();
    //qDebug() << keysCombination;
    emit press(event->key());
    //    QQuickItem *keyboard = ui->quickWidget->rootObject();
    //    QVariant key = event->key();
    //    QVariant retValue;
    //    QMetaObject::invokeMethod(keyboard, "pressKey", Q_RETURN_ARG(QVariant, retValue), Q_ARG(QVariant, key));

    if (keysCombination == "") {
        // for shift, ctrl, alt
        return;
    }
    if (event->key() == Qt::Key_Backspace) {
        for (auto handler: handlers) {
            handler->backspacePressed();
        }
        emit keyPressed();
        return;
    }
    for (auto handler: handlers) {
        handler->keyPressed(keysCombination[0]);
    }
    emit keyPressed();
}

void GameWindow::keyReleaseEvent(QKeyEvent *event) {
    emit release(event->key());
    //    QQuickItem *keyboard = ui->quickWidget->rootObject();
    //    QVariant key = event->key();
    //    QVariant retValue;
    //    QMetaObject::invokeMethod(keyboard, "releaseKey", Q_RETURN_ARG(QVariant, retValue), Q_ARG(QVariant, key));
}

void GameWindow::keyPressed() {
    ui->userText->setText(main_handler->getBuffer());
    if (main_handler->getErrorStatus())
        setError();
    else
        unsetError();
    ui->userText->setPalette(palette);
    if (main_handler->isEnded()) {
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
