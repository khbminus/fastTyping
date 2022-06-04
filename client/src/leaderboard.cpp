#include "leaderboard.h"
#include "ui_leaderboard.h"

Leaderboard::Leaderboard(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::Leaderboard),
      controller(new LeaderboardController(this)) {
    ui->setupUi(this);
    auto tabs = controller->getDictionariesTabs();
    for (const auto &[tab, name] : tabs) {
        ui->tabWidget->addTab(tab, name);
    }
}

Leaderboard::~Leaderboard() {
    delete ui;
}

void Leaderboard::on_ReturnButton_clicked() {
    auto &holder = LeaderboardHolder::getInstance();
    auto &windowController = FastTyping::WindowController::getInstance();
    holder.dropWindow();
    windowController.setActiveWindow("MainWindow");
}