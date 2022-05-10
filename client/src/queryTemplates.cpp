#include "queryTemplates.h"
#include "common/include/json.hpp"

#include <iostream>

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

    QString leave_query() {
        json result;
        result["header"] = {{"type", "leaveGame"}};
        result["body"] = json::object();
        return dump(result);
    }

    QString buffer_clear_query() {
        json result;
        result["header"] = {{"type", "clearBuffer"}};
        result["body"] = json::object();
        return dump(result);
    }

    QString new_word_query() {
        json result;
        result["header"] = {{"type", "getNewWord"}};
        result["body"] = json::object();
        return dump(result);
    }


    QString create_game_query(bool auto_join) {
        json result;
        json words = json::array({"This", "is", "sample", "don't", "judge", "me"});

        result["header"] = {{"type", "createGame"}};
        result["body"] = {{"dictionaryName", "const"},
                          {"parserName", "simple"},
                          {"autoJoin", auto_join},
                          {"words", words}};
        return dump(result);
    }
}// namespace client::queries