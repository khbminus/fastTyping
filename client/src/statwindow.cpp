#include "statwindow.h"
#include "confirmWindow.h"
#include "gameContextManager.h"
#include "queryTemplates.h"
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
