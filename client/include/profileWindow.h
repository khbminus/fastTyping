#ifndef PROFILEWINDOW_H
#define PROFILEWINDOW_H

#include <QMainWindow>
#include <QQuickView>
#include "gameManager.h"
#include "profileDataModel.h"
#include "textScreen.h"
#include "windowcontroller.h"

namespace Ui {
class StatWindow;
}

class ProfileWindow : public QQuickView {
    Q_OBJECT

public:
    explicit ProfileWindow(QWindow *parent = nullptr);

private slots:
    void onReturnButtonClicked();

private:
    ProfileDataModel *dataModel;
    ProfileTableModel *tableModel;
};

class ProfileHolder {
public:
    static ProfileHolder &getInstance() {
        static ProfileHolder holder;
        return holder;
    }

    void createWindow() {
        window.reset(new ProfileWindow);
        auto &controller = FastTyping::WindowController::getInstance();
        controller.registerWindow("ProfileWindow", window.get());
    }
    void dropWindow() {
        auto &controller = FastTyping::WindowController::getInstance();
        controller.dropWindow("ProfileWindow");
        window.reset();
    }

private:
    ProfileHolder() = default;
    QScopedPointer<ProfileWindow, QScopedPointerDeleteLater> window;
};

#endif  // PROFILEWINDOW_H
