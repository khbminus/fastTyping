#include "profileWindow.h"
#include <QQuickItem>
#include <chrono>
#include <nlohmann/json.hpp>
#include "profileDataModel.h"
#include "windowcontroller.h"

ProfileWindow::ProfileWindow(QWindow *parent)
    : QQuickView(parent), model(new ProfileDataModel(this)) {
    qDebug() << model;
    setInitialProperties(
        {{"userName", "Абобамейтсер"},
         {"mainInfo", QVariant::fromValue(model->commonData())},
         {"dictsNames", QVariant::fromValue(model->dictionariesNames())},
         {"dictsValues", QVariant::fromValue(model->dictionaries())}});
    setSource(QUrl("qrc:/ProfileWindow.qml"));
    connect(rootObject(), SIGNAL(returnPressed()), this,
            SLOT(onReturnButtonClicked()));
}

void ProfileWindow::onReturnButtonClicked() {
    auto &holder = ProfileHolder::getInstance();
    auto &windowController = FastTyping::WindowController::getInstance();
    holder.dropWindow();
    windowController.setActiveWindow("MainWindow");
}