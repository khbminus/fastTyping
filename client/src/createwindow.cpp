#include "createwindow.h"
#include <algorithm>
#include <nlohmann/json.hpp>
#include "errorHandler.h"
#include "gameContextManager.h"
#include "queryTemplates.h"
#include "responseParse.h"
#include "sonicSocket.h"
#include "ui_createwindow.h"
#include "windowcontroller.h"

CreateGameWindow::CreateGameWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::CreateWindow) {
    ui->setupUi(this);
}

CreateGameWindow::~CreateGameWindow() {
    delete ui;
}

void CreateGameWindow::on_ReturnButton_clicked() {
    auto &controller = FastTyping::WindowController::getInstance();
    controller.setActiveWindow("MainWindow");
}

void CreateGameWindow::on_CreateButton_clicked() {
    using client::responses::ensure_success;
    using client::responses::error_text;
    using nlohmann::json;

    using client::queries::create_game_query;
    using client::web::socket;
    bool isSolo = false;
    if (ui->ModeBox->currentText() == "Single player") {
        isSolo = true;
    }
    QString raw_response = socket().query(
        create_game_query(ui->WordsBox->currentText(), true, isSolo));
    qDebug() << "create result:" << raw_response;
    json response = json::parse(raw_response.toStdString());

    if (ensure_success(response)) {
        auto &context = ContextManager::get_instance();
        context.set_context_from_create_query(response, isSolo);
        auto &controller = FastTyping::WindowController::getInstance();
        controller.setActiveWindow("StartGameWindow");
    }
}

void CreateGameWindow::showEvent(QShowEvent *event) {
    using client::queries::get_dictionaries_query;
    using client::web::socket;
    using nlohmann::json;

    QWidget::showEvent(event);

    QString raw_response = socket().query(get_dictionaries_query());
    qDebug() << "list of dictionaries: " << raw_response;
    json response = json::parse(raw_response.toStdString());
    std::vector<std::string> raw_dictionaries =
        response["body"]["list"].get<std::vector<std::string>>();
    QStringList dictionaries;
    std::transform(raw_dictionaries.begin(), raw_dictionaries.end(),
                   std::back_inserter(dictionaries), QString::fromStdString);
    ui->WordsBox->clear();
    ui->WordsBox->addItems(dictionaries);
}
