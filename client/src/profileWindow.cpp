#include "profileWindow.h"
#include <QQuickItem>
#include <chrono>
#include <nlohmann/json.hpp>
#include "profileDataModel.h"
#include "windowcontroller.h"

ProfileWindow::ProfileWindow(QWindow *parent)
    : QQuickView(parent),
      dataModel(new ProfileDataModel(this)),
      tableModel(new ProfileTableModel(this)) {
    setInitialProperties(
        {{"userName", "Абобамейтсер"},
         {"mainInfo", QVariant::fromValue(dataModel->commonData())},
         {"dictsNames", QVariant::fromValue(dataModel->dictionariesNames())},
         {"dictsValues", QVariant::fromValue(dataModel->dictionaries())},
         {"tableModel", QVariant::fromValue(tableModel)}});
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