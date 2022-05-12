#ifndef RESPONSE_HANDLER_H
#define RESPONSE_HANDLER_H


#include "socketWrapper.h"
#include <QString>

namespace client::responses {

    class APIHandler : public QObject, public client::web::ResponseHandler {
        Q_OBJECT
    public:
        client::web::ResponseType type(QString const &line) const override;
        void handle(QString const &line) override;
    signals:
        void error_signal();
        void correct_signal();
        void end_signal();
        void correct_word_signal();
    };
}// namespace client::responses

#endif
