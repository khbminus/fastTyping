#include "profileWindow.h"
#include <QQuickItem>
#include <chrono>
#include <nlohmann/json.hpp>
#include "profileDataModel.h"
#include "windowcontroller.h"

ProfileWindow::ProfileWindow(QWindow *parent) : QQuickView(parent) {
    setSource(QUrl("qrc:/ProfileWindow.qml"));
    connect(rootObject(), SIGNAL(returnPressed()), this,
            SLOT(onReturnButtonClicked()));
}

void ProfileWindow::onReturnButtonClicked() {
    auto &windowController = FastTyping::WindowController::getInstance();
    windowController.setActiveWindow("MainWindow");
}