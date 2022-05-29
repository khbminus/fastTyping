#include "gamewindow.h"
#include <qqml.h>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>
#include <iostream>
#include "confirmWindow.h"
#include "gameContextManager.h"
#include "keyboard.h"
#include "queryTemplates.h"
#include "sonicSocket.h"
#include "textScreen.h"
#include "ui_gamewindow.h"
#include "windowcontroller.h"
using FastTyping::TextScreen::TextListModel;
GameWindow::GameWindow(const std::vector<GameManager *> &managers,
                       GameManager *manager,
                       QWindow *parent)
    : QQuickView(parent), textModel(TextListModel(manager->blob(), this)) {
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
    QQuickView::setGraphicsApi(QSGRendererInterface::OpenGL);

    setInitialProperties(
        {{"keyModel", QVariant::fromValue(
                          &FastTyping::Keyboard::KeyboardModel::getInstance())},
         {"textModel", QVariant::fromValue(&textModel)}});
    setSource(QUrl(QString::fromUtf8("qrc:/GameWindow.qml")));

    connect(this, SIGNAL(press(QVariant)), rootObject(),
            SLOT(pressKey(QVariant)));
    connect(this, SIGNAL(release(QVariant)), rootObject(),
            SLOT(releaseKey(QVariant)));
    connect(this, SIGNAL(highlight(QVariant)), rootObject(),
            SLOT(highlightKey(QVariant)));
    connect(this, SIGNAL(shiftHighlight()), rootObject(),
            SLOT(shiftHighlight()));
    connect(this, SIGNAL(backspaceHighlight()), rootObject(),
            SLOT(backspaceHighlight()));
    connect(this, SIGNAL(clearHighlight()), rootObject(),
            SLOT(clearHighlight()));
    connect(&textModel, SIGNAL(cursorMoved(QVariant)), rootObject(),
            SLOT(moveCursor1(QVariant)));

    QObject::connect(rootObject(), SIGNAL(pressedKey(int, QString)), this,
                     SLOT(keyPress(int, QString)));
    QObject::connect(rootObject(), SIGNAL(releasedKey(int, QString)), this,
                     SLOT(keyRelease(int, QString)));
    QObject::connect(rootObject(), SIGNAL(returnPressed()), this,
                     SLOT(on_ReturnButton_clicked()));

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
    QObject::connect(manager, &GameManager::print_signal, &textModel,
                     &TextListModel::onMove);
    QObject::connect(manager, &GameManager::errorOnPositionSignal, &textModel,
                     &TextListModel::onWrongChar);
    QObject::connect(manager, &GameManager::correctOnPositionSignal, &textModel,
                     &TextListModel::onCorrectChar);
    highlightNextKey();
}

void GameWindow::keyPress(int key, QString keysCombination) {
    emit press(key);
    if (keysCombination == "") {
        return;
    }

    if (key == Qt::Key_Backspace) {
        emit backspace_pressed();
    } else {
        emit key_pressed(keysCombination[0]);
    }
}

void GameWindow::keyRelease(int key, QString text) {
    emit release(key);
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

void GameWindow::error_slot() {
    emit clearHighlight();
    emit backspaceHighlight();
}

void GameWindow::correct_slot() {
    highlightNextKey();
}

void GameWindow::end() {
    auto &controller = FastTyping::WindowController::getInstance();
    textModel.stopTimer();
    controller.setActiveWindow("StatWindow");
}

void GameWindow::highlightNextKey() {
    auto &keyboard = FastTyping::Keyboard::KeyboardModel::getInstance();
    auto next = ContextManager::get_instance().get_local_manager()->next();
    emit clearHighlight();
    if (next.isValid() && keyboard.isUpper(next.toChar())) {
        emit shiftHighlight();
        next.setValue(next.toChar().toLower());
    }
    emit highlight(next);
}
