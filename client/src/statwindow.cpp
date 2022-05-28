#include "statwindow.h"
#include "confirmWindow.h"
#include "gameContextManager.h"
#include "queryTemplates.h"
#include "responseParse.h"
#include "errorHandler.h"
#include "sonicSocket.h"
#include "ui_statwindow.h"
#include "windowcontroller.h"

StatWindow::StatWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::StatWindow) {
    ui->setupUi(this);
}

StatWindow::~StatWindow() {
    delete ui;
}

void StatWindow::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    using client::queries::get_game_stat_query;
    using client::responses::ensure_success;
    using client::responses::error_text;
    using client::web::socket;
    using nlohmann::json;
    QString raw_response = socket().query(get_game_stat_query());
    qDebug() << "statistics: " << raw_response;
    json response = json::parse(raw_response.toStdString());
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
