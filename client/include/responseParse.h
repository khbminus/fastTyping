#ifndef RESPONSE_PARSE_H
#define RESPONSE_PARSE_H

#include <nlohmann/json.hpp>
#include <QString>

namespace client::responses {
bool is_success(nlohmann::json const& response);
QString error_text(nlohmann::json const &response);
}

#endif
