#include "statwindow.h"
#include <QQuickItem>
#include <chrono>
#include <nlohmann/json.hpp>
#include "confirmWindow.h"
#include "errorHandler.h"
#include "gameContextManager.h"
#include "queryTemplates.h"
#include "responseParse.h"
#include "sonicSocket.h"
#include "ui_statwindow.h"
#include "windowcontroller.h"

StatWindow::StatWindow(GameManager *manager, QWindow *parent)
    : QQuickView(parent), textModel(manager->blob() + " ", this) {
    using namespace std::literals;
    using client::queries::get_game_stat_query;
    using nlohmann::json;

    auto stats = json::parse(
        client::web::socket().query(get_game_stat_query()).toStdString());
    qDebug() << QString::fromUtf8(stats.dump());
    textModel.startTimer();
    auto model = dynamic_cast<LocalManager *>(manager)->getModel();
    for (int i = 0; i < model->rowCount(QModelIndex()); i++) {
        qDebug() << model->data(model->index(i, 0))
                 << model->data(model->index(i, 1))
                 << model->data(model->index(i, 2))
                 << model->data(model->index(i, 3));
    }
    setInitialProperties(
        {{"textModel", QVariant::fromValue(&textModel)},
         {"rawWPM", QString::number(stats["body"]["rawWPM"].get<int>())},
         {"onlyCorrectWPM", stats["body"]["WPM"].get<int>()},
         {"charsTyped", stats["body"]["totalChars"].get<int>()},
         {"charsCorrect", stats["body"]["correctChars"].get<int>()},
         {"chartModel", QVariant::fromValue(model)},
         {"maxChartWPM", model->getMaxWPM()},
         {"maxChartErrors", model->getMaxErrors()},
         {"gameId", ContextManager::get_instance().get_game_id()}});
    setSource(QUrl(QString::fromUtf8("qrc:/StatWindow.qml")));

    QObject::connect(rootObject(), SIGNAL(returnPressed()), this,
                     SLOT(on_ReturnButton_clicked()));
    textModel.setCursorPosition(-1);
}

void StatWindow::on_ReturnButton_clicked() {
    using client::queries::leave_query;
    using client::web::socket;

    if (confirm("Exit", "Are you really want to exit")) {
        ContextManager::get_instance().reset_context();
        qDebug() << "leave result: " << socket().query(leave_query());
        auto &controller = FastTyping::WindowController::getInstance();
        controller.setActiveWindow("MainWindow");
    }
}
