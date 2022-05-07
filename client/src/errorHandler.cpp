#include "errorHandler.h"
#include <QMessageBox>
#include <QAbstractButton>
#include <QPushButton>
#include <cstdlib>

void error_alert(QString title, QString text, bool is_blocking) {
    QMessageBox message_box;
    message_box.setText(text);
    message_box.setWindowTitle(title);

    QPushButton* abort = message_box.addButton(QMessageBox::Abort);

    if (!is_blocking) {
        message_box.addButton("continue", QMessageBox::ActionRole);
    }

    message_box.exec();


    if (message_box.clickedButton() == abort) {
        std::exit(0);
    }
}
