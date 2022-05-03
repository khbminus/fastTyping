#include "queryTemplates.h"
#include "common/include/json.hpp"

using nlohmann::json;

QString dump(json const& data) {
    return QString::fromStdString(data.dump() + "\n");

}

namespace client::queries {
QString greeting_query(QString const& name) {
    json result;
    result["header"] = {{"type", "hello"}};
    result["body"] = {{"name", name.toStdString()}};
    return dump(result);
}

QString key_pressed_query(QString const& buffer) {
    json result;
    result["header"] = {{"type", "addNewChar"}};
    result["body"] = {{"char", buffer.toStdString()}};
    return dump(result);
}

QString join_query(int id) {
    json result;
    result["header"] = {{"type", "joinGame"}};
    result["body"] = {{"id", id}};
    return dump(result);
}
}
