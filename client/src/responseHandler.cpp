#include <map>
#include <string>
#include "responseHandler.h"
#include "common/include/json.hpp"

namespace client::responses {
using namespace client::web;
using nlohmann::json;

std::map<std::string, ResponseType> header_to_type {
    {"hello", ResponseType::blocking},
    {"joinGame", ResponseType::blocking},
    {"addNewChar", ResponseType::async}
};

ResponseType APIHandler::type(QString const& line) const {
    json response = json::parse(line.toStdString());
    std::string header = response["header"]["type"].get<std::string>();
    if (header_to_type.count(header)) {
        return header_to_type[header];
    }
    return ResponseType::async;
}

void APIHandler::handle(QString const& line) {
}

}

