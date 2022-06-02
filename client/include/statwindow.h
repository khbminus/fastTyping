#ifndef STATWINDOW_H
#define STATWINDOW_H

#include <QMainWindow>
#include <QQuickView>
#include "gameManager.h"
#include "textScreen.h"

namespace Ui {
class StatWindow;
}

class StatWindow : public QQuickView {
    Q_OBJECT

public:
    explicit StatWindow(GameManager *manager, QWindow *parent = nullptr);

private slots:
    void on_ReturnButton_clicked();

private:
    FastTyping::TextScreen::TextListModel textModel;
};

#endif  // STATWINDOW_H
