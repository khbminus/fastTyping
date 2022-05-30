#include "statwindow.h"
#include <QQuickItem>
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
    : QQuickView(parent),
      textModel(FastTyping::TextScreen::TextListModel(manager->blob(), this)) {
    using client::queries::get_game_stat_query;
    using nlohmann::json;

    auto stats = json::parse(
        client::web::socket().query(get_game_stat_query()).toStdString());
    qDebug() << QString::fromUtf8(stats.dump());

    setInitialProperties({
        {"textModel", QVariant::fromValue(&textModel)},
        {"rawWPM", QString::number(stats["body"]["rawWPM"].get<int>())},
        {"onlyCorrectWPM", QString::number(stats["body"]["WPM"].get<int>())},
    });
    setSource(QUrl(QString::fromUtf8("qrc:/StatWindow.qml")));

    QObject::connect(rootObject(), SIGNAL(returnPressed()), this,
                     SLOT(on_ReturnButton_clicked()));
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
