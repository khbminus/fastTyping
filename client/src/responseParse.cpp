#include "responseParse.h"
#include <QDebug>
#include <QString>
#include <map>
#include <string>
#include "errorHandler.h"

using nlohmann::json;

namespace client::responses {

std::map<std::string, std::string> success_type{
    {"createGame", "GameCreatedSuccessfully"},
    {"joinGame", "GameJoinedSuccessfully"},
    {"leaveGame", "GameLeaveSuccessfully"},
    {"backspace", "checkResult"},
    {"addNewChar", "checkResult"},
    {"login", "success"},
    {"register", "success"},
    {"getStates", "currentState"}};

std::map<std::string, std::map<std::string, QString>> error_texts{
    {"createGame",
     {{"alreadyInGame", "You are already in game. Wait until it ends"},
      {"wrongFormatError", "Invalid parser or dictionary"}}},
    {"joinGame",
     {{"alreadyInGame", "You are already in game. Wait until it ends"},
      {"wrongFormatError", "Internal error"},
      {"connectAfterLeaveError", "You are trying to join game after leave"},
      {"wrongIdError", "There no game with this Id"}}},
    {"register", {{"nameAlreadyExists", "User with this name already exists"}}},
    {"login", {{"incorrectName", "Invalid name or password"}}}};

std::map<std::string, QString> error_titles{
    {"createGame", "Error on game create"},
    {"joinGame", "Error on game join"}};

bool is_success(json const &response) {
    return response["header"]["type"].get<std::string>() ==
           success_type[response["header"]["queryType"].get<std::string>()];
}

QString error_text(json const &response) {
    // There should be more complicated logic
    return error_texts[response["header"]["queryType"].get<std::string>()]
                      [response["header"]["type"].get<std::string>()];
}

QString error_title(json const &response) {
    // There should be more complicated logic
    return error_titles[response["header"]["queryType"].get<std::string>()];
}

bool ensure_success(nlohmann::json const &response) {
    if (is_success(response)) {
        return true;
    }
    error_alert(error_title(response), error_text(response));
    return false;
}
}  // namespace client::responses
