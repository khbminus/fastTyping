//
// Created by doktorkrab on 3/11/22.
//

#ifndef FASTTYPING_WINDOWCONTROLLER_H
#define FASTTYPING_WINDOWCONTROLLER_H
#include <QMainWindow>
#include <QObject>
#include <QQuickView>
#include <QSharedPointer>
#include <QString>
#include <string>
#include <unordered_map>

namespace FastTyping {
struct WindowController : QObject {
    Q_OBJECT
public:
    static WindowController &getInstance() {
        static WindowController controller;
        return controller;
    }

    void registerWindow(const std::string &windowName, QMainWindow *window);
    void registerWindow(const std::string &windowName, QQuickView *window);
    void setActiveWindow(const std::string &windowName);
    void dropWindow(std::string const &window_name);

private:
    std::unordered_map<std::string, QMainWindow *> windows;
    std::unordered_map<std::string, QQuickView *> windowsQuick;
    QMainWindow *activeWindow;
    QQuickView *activeQuickWindow;

    WindowController() = default;
};
};  // namespace FastTyping

#endif  // FASTTYPING_WINDOWCONTROLLER_H
