#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QQuickView>
#include <QQuickWidget>
#include <QVariant>
#include <memory>
#include "gameManager.h"
#include "textScreen.h"

namespace Ui {
class GameWindow;
}

class GameWindow : public QQuickView {
    Q_OBJECT

public:
    explicit GameWindow(const std::vector<GameManager *> &managers,
                        GameManager *manager,
                        QWindow *parent = nullptr);

private:
    FastTyping::TextScreen::TextListModel textModel;

    void highlightNextKey();
signals:
    void press(QVariant key);
    void release(QVariant key);
    void highlight(QVariant key);
    void shiftHighlight();
    void backspaceHighlight();
    void clearHighlight();
    void backspace_pressed();
    void key_pressed(QChar button);
private slots:
    void on_ReturnButton_clicked();
    void error_slot();
    void correct_slot();
    void end();
    void keyPress(int, QString);
    void keyRelease(int, QString);
};

#endif  // GAMEWINDOW_H
