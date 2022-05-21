#ifndef RESPONSE_PARSE_H
#define RESPONSE_PARSE_H

#include <QString>
#include <nlohmann/json.hpp>

namespace client::responses {
bool is_success(nlohmann::json const &response);
QString error_text(nlohmann::json const &response);
QString error_title(nlohmann::json const &response);
bool ensure_success(nlohmann::json const &response);
}  // namespace client::responses

#endif
