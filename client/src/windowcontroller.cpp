#include "windowcontroller.h"
#include <utility>

namespace FastTyping {
void WindowController::setActiveWindow(const std::string &windowName) {
    if (activeWindow != nullptr) {
        activeWindow->close();
        activeWindow = nullptr;
    } else if (activeQuickWindow != nullptr) {
        activeQuickWindow->close();
        activeQuickWindow = nullptr;
    }
    if (windows.count(windowName) == 1) {
        activeWindow = windows[windowName];
        activeWindow->show();
    } else {
        activeQuickWindow = windowsQuick[windowName];
    }
}

void WindowController::registerWindow(const std::string &windowName,
                                      QMainWindow *window) {
    windows[windowName] = window;
}
void WindowController::registerWindow(const std::string &windowName,
                                      QQuickView *window) {
    windowsQuick[windowName] = window;
}

void WindowController::dropWindow(std::string const &window_name) {
    if (windows.count(window_name) == 1) {
        windows[window_name] = nullptr;
    } else {
        windowsQuick[window_name] = nullptr;
    }
}

}  // namespace FastTyping
