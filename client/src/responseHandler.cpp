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
        json response = json::parse(line.toStdString());
        if (response["header"]["queryType"].get<std::string>() == "addNewChar" ||
            response["header"]["queryType"].get<std::string>() == "backspace") {
            if (response["body"]["isEnd"].get<bool>()) {
                emit end_signal();
            } else if (response["body"]["isFullCorrect"].get<bool>()) {
                emit correct_word_signal();
            } else if (response["body"]["isPrefixCorrect"].get<bool>()) {
                emit correct_signal();
            } else {
                emit error_signal();
            }
        }
    }
}// namespace client::responses
