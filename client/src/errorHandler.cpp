#include "errorHandler.h"
#include <QAbstractButton>
#include <QMessageBox>
#include <QPushButton>
#include <cstdlib>

// cppcheck-suppress unusedFunction
void error_alert(QString title,
                 QString text,
                 bool has_close_button,
                 bool has_exit_button) {
    if (!has_close_button && !has_exit_button) {
        has_exit_button = true;
    }

    QMessageBox message_box;
    message_box.setText(text);
    message_box.setWindowTitle(title);

    QPushButton *exit_button = nullptr;
    // QPushButton* close_button = nullptr;

    if (has_exit_button) {
        exit_button = message_box.addButton(QMessageBox::Abort);
    }

    if (has_close_button) {
        message_box.addButton("continue", QMessageBox::ActionRole);
    }

    message_box.exec();

    if (message_box.clickedButton() == exit_button) {
        std::exit(0);
    }
}
