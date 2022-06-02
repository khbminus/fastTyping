#ifndef PROFILEWINDOW_H
#define PROFILEWINDOW_H

#include <QMainWindow>
#include <QQuickView>
#include "gameManager.h"
#include "textScreen.h"

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
};

#endif  // PROFILEWINDOW_H
