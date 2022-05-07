#include "responseHandler.h"
#include "common/include/json.hpp"
#include <iostream>
#include <map>
#include <string>

namespace client::responses {
    using namespace client::web;
    using nlohmann::json;

    std::map<std::string, ResponseType> header_to_type{
            {"joinGame", ResponseType::blocking},
            {"hello", ResponseType::blocking},
            {"leaveGame", ResponseType::blocking},
            {"createGame", ResponseType::blocking},
            {"addNewChar", ResponseType::async},
            {"addBackspace", ResponseType::async},
    };

    ResponseType APIHandler::type(QString const &line) const {
        json response = json::parse(line.toStdString());
        std::string header = response["header"]["queryType"].get<std::string>();
        if (header_to_type.count(header)) {
            return header_to_type[header];
        }
        return ResponseType::async;
    }

    void APIHandler::handle(QString const &line) {
        std::cout << line.toStdString() << std::endl;
    }
}// namespace client::responses
