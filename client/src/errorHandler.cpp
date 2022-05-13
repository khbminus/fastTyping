#include "errorHandler.h"
#include <QAbstractButton>
#include <QMessageBox>
#include <QPushButton>
#include <cstdlib>

// cppcheck-suppress unusedFunction
void error_alert(QString title, QString text, bool is_blocking) {
    QMessageBox message_box;
    message_box.setText(text);
    message_box.setWindowTitle(title);

    QPushButton *abort = message_box.addButton(QMessageBox::Abort);

    if (!is_blocking) {
        message_box.addButton("continue", QMessageBox::ActionRole);
    }

    message_box.exec();

    if (message_box.clickedButton() == abort) {
        std::exit(0);
    }
}
