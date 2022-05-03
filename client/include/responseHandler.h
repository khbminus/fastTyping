#ifndef RESPONSE_HANDLER_H
#define RESPONSE_HANDLER_H


#include <QString>
#include "socketWrapper.h"

namespace client::responses {

class APIHandler : public client::web::ResponseHandler{
    client::web::ResponseType type(QString const& line) const override;
    void handle(QString const& line) override;
};

}

#endif
