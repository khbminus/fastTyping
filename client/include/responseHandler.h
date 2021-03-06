#ifndef RESPONSE_HANDLER_H
#define RESPONSE_HANDLER_H

#include <QString>
#include <nlohmann/json.hpp>
#include "socketWrapper.h"

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
    void stateUpdated(const nlohmann::json &);
    void gameWaited();
};

APIHandler &handler();
}  // namespace client::responses

#endif
