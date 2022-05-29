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
                       QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::GameWindow),
      main_manager(manager),
      textOut(new QQuickWidget(this)),
      textModel(TextListModel(manager->blob(), this)) {
    ui->setupUi(this);
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
    QQuickView::setGraphicsApi(QSGRendererInterface::OpenGL);
    ui->quickWidget->rootObject()->setProperty(
        "keyModel", QVariant::fromValue(
                        &FastTyping::Keyboard::KeyboardModel::getInstance()));

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
    connect(this, SIGNAL(press(QVariant)), ui->quickWidget->rootObject(),
            SLOT(pressKey(QVariant)));
    connect(this, SIGNAL(release(QVariant)), ui->quickWidget->rootObject(),
            SLOT(releaseKey(QVariant)));
    connect(this, SIGNAL(highlight(QVariant)), ui->quickWidget->rootObject(),
            SLOT(highlightKey(QVariant)));
    connect(this, SIGNAL(shiftHighlight()), ui->quickWidget->rootObject(),
            SLOT(shiftHighlight()));
    connect(this, SIGNAL(backspaceHighlight()), ui->quickWidget->rootObject(),
            SLOT(backspaceHighlight()));
    connect(this, SIGNAL(clearHighlight()), ui->quickWidget->rootObject(),
            SLOT(clearHighlight()));

    ui->game_id->setText(
        QString::number(ContextManager::get_instance().get_game_id()));
    textOut->setSource(QUrl(QString::fromUtf8("qrc:/textScreen.qml")));
    textOut->rootObject()->setProperty("model",
                                       QVariant::fromValue(&textModel));

    connect(&textModel, SIGNAL(cursorMoved(QVariant)), textOut->rootObject(),
            SLOT(moveCursor1(QVariant)));

    textOut->setGeometry(QRect(50, 30, 701, 51));
    //    textOut->rootObject()->setProperty("width", 701);
    //    textOut->rootObject()->setProperty("height", 51);

    highlightNextKey();
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
    emit press(event->key());
    if (keysCombination == "") {
        return;
    }

    if (event->key() == Qt::Key_Backspace) {
        emit backspace_pressed();
    } else {
        emit key_pressed(keysCombination[0]);
    }
}

void GameWindow::keyReleaseEvent(QKeyEvent *event) {
    emit release(event->key());
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
