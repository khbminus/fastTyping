#ifndef RESPONSE_HANDLER_H
#define RESPONSE_HANDLER_H


#include "socketWrapper.h"
#include <QString>

namespace client::responses {

    class APIHandler : public client::web::ResponseHandler {
        client::web::ResponseType type(QString const &line) const override;
        void handle(QString const &line) override;
    };

}// namespace client::responses

#endif
