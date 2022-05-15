#include "responseParse.h"
#include <QString>
#include <map>
#include <string>

using nlohmann::json;

namespace client::responses {

std::map<std::string, std::string> success_type{
    {"login", "loginSuccessfully"},
    {"createGame", "GameCreatedSuccessfully"},
    {"joinGame", "GameJoinedSuccessfully"},
    {"leaveGame", "GameLeaveSuccessfully"},
};

std::map<std::string, std::map<std::string, QString>> error_texts{
    {"createGame",
     {{"alreadyInGame", "You are already in game. Wait until it ends"},
      {"wrongFormatError", "Invalid parser or dictionary"}}},
    {"joinGame",
     {{"alreadyInGame", "You are already in game. Wait until it ends"},
      {"wrongFormatError", "Internal error"},
      {"connectAfterLeaveError", "You are trying to join game after leave"},
      {"wrongIdError", "There no game with this Id"}}}};

bool is_success(json const &response) {
    return response["header"]["type"].get<std::string>() ==
           success_type[response["header"]["queryType"].get<std::string>()];
}

QString error_text(json const &response) {
    // There should be more complicated logic
    return error_texts[response["header"]["queryType"].get<std::string>()]
                      [response["header"]["type"].get<std::string>()];
}
}  // namespace client::responses
