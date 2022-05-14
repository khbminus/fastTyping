#include "confirmWindow.h"
#include <QAbstractButton>
#include <QMessageBox>
#include <QPushButton>
#include <cstdlib>

// cppcheck-suppress unusedFunction
bool confirm(QString title, QString text) {
    QMessageBox message_box;
    message_box.setText(text);
    message_box.setWindowTitle(title);

    QPushButton *yes = message_box.addButton(QMessageBox::Yes);
    [[maybe_unused]]QPushButton *no = message_box.addButton(QMessageBox::No);

    message_box.exec();

    return message_box.clickedButton() == yes;
}
