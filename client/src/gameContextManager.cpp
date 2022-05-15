#include "gameContextManager.h"
#include "windowcontroller.h"

using nlohmann::json;

int ContextManager::get_user_id() const {
    return user_id;
}

void ContextManager::set_user_id(int new_id) {
    user_id = new_id;
}

void ContextManager::set_context_from_create_query(
    const nlohmann::json &create_query) {}
void ContextManager::set_context_from_join_query(
    const nlohmann::json &create_query) {}
