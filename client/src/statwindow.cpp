#include "statwindow.h"
#include <QQuickItem>
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
    setInitialProperties({{"textModel", QVariant::fromValue(&textModel)}});
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
