#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <QMainWindow>
#include "leaderboardController.h"
#include "windowcontroller.h"

namespace Ui {
class Leaderboard;
}

class Leaderboard : public QMainWindow {
    Q_OBJECT

public:
    explicit Leaderboard(QWidget *parent = nullptr);
    ~Leaderboard() override;
private slots:
    void on_ReturnButton_clicked();

private:
    Ui::Leaderboard *ui;
    LeaderboardController *controller;
};

class LeaderboardHolder {
public:
    static LeaderboardHolder &getInstance() {
        static LeaderboardHolder holder;
        return holder;
    }

    void createWindow() {
        window.reset(new Leaderboard);
        auto &controller = FastTyping::WindowController::getInstance();
        controller.registerWindow("Leaderboard", window.get());
    }
    void dropWindow() {
        auto &controller = FastTyping::WindowController::getInstance();
        controller.dropWindow("Leaderboard");
        window.reset();
    }

private:
    LeaderboardHolder() = default;
    QScopedPointer<Leaderboard, QScopedPointerDeleteLater> window;
};

#endif  // LEADERBOARD_H
