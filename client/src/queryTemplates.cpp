#include "queryTemplates.h"
#include <iostream>
#include <nlohmann/json.hpp>

using nlohmann::json;

QString dump(json const &data) {
    return QString::fromStdString(data.dump() + "\n");
}

namespace client::queries {
QString greeting_query(QString const &name) {
    json result;
    result["header"] = {{"type", "hello"}};
    result["body"] = {{"name", name.toStdString()}};
    return dump(result);
}

QString key_pressed_query(QString const &new_char) {
    json result;
    result["header"] = {{"type", "addNewChar"}};
    result["body"] = {{"char", new_char.toStdString()}};
    return dump(result);
}

QString get_dictionaries_query() {
    json result;

    result["header"] = {{"type", "getDictionaries"}};
    result["body"] = json::object();
    return dump(result);
}

QString backspace_pressed_query() {
    json result;
    result["header"] = {{"type", "backspace"}};
    result["body"] = json::object();
    return dump(result);
}

QString join_query(int id) {
    json result;
    result["header"] = {{"type", "joinGame"}};
    result["body"] = {{"id", id}};
    return dump(result);
}

QString start_query() {
    json result;
    result["header"] = {{"type", "startGame"}};
    result["body"] = json::object();
    return dump(result);
}

QString wait_game_query() {
    json result;
    result["header"] = {{"type", "waitGameStart"}};
    result["body"] = json::object();
    return dump(result);
}

QString leave_query() {
    json result;
    result["header"] = {{"type", "leaveGame"}};
    result["body"] = json::object();
    return dump(result);
}

QString sign_in_query(QString const &username, QString const &password) {
    json result;

    result["header"] = {{"type", "login"}};
    result["body"] = {{"name", username.toStdString()},
                      {"password", password.toStdString()}};
    return dump(result);
}

QString sign_on_query(QString const &username, QString const &password) {
    json result;

    result["header"] = {{"type", "register"}};
    result["body"] = {{"name", username.toStdString()},
                      {"password", password.toStdString()}};
    return dump(result);
}

QString get_line_query() {
    json result;

    result["header"] = {{"type", "getNewLine"}};
    result["body"] = json::object();
    return dump(result);
}

QString create_game_query(QString const &dict, bool auto_join, bool isSolo) {
    json result;
    json words = json::array({"This"});

    result["header"] = {{"type", "createGame"}};
    result["body"] = {{"dictionaryName", dict.toStdString()},
                      {"parserName", (isSolo ? "solo" : "simple")},
                      {"autoJoin", auto_join},
                      {"words", words}};
    return dump(result);
}

QString user_finish_query() {
    json result;
    result["header"] = {{"type", "userFinished"}};
    result["body"] = {};
    return dump(result);
}

QString get_game_stat_query() {
    json result;
    result["header"] = {{"type", "getGameStatistics"}};
    result["body"] = {};
    return dump(result);
}

QString getStatesQuery() {
    json result;
    result["header"] = {{"type", "getStates"}};
    result["body"] = json::object();
    return dump(result);
}
}  // namespace client::queries
