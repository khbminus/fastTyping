#include "windowcontroller.h"
#include <utility>
namespace FastTyping {
void WindowController::setActiveWindow(const std::string &windowName) {
    if (activeWindow) {
        activeWindow->close();
    }
    activeWindow = windows[windowName];
    activeWindow->show();
}

void WindowController::registerWindow(const std::string &windowName,
                                      QSharedPointer<QMainWindow> window) {
    windows[windowName] = std::move(window);
}
}  // namespace FastTyping