//
// Created by doktorkrab on 3/11/22.
//
#include "windowcontroller.h"

#include <utility>
namespace FastTyping {
    void WindowController::setActiveWindow(const QString &windowName) {
        if (activeWindow) {
            activeWindow->close();
        }
        activeWindow = windows[windowName];
        activeWindow->show();
    }

    void WindowController::registerWindow(const QString &windowName, QSharedPointer<QMainWindow> window) {
        windows[windowName] = std::move(window);
    }
}// namespace FastTyping