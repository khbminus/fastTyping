#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <QString>

void error_alert(QString title,
                 QString text,
                 bool has_close_button = true,
                 bool has_exit_button = false);

#endif
