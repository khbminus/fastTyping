#include "gameContextManager.h"

int ContextManager::get_user_id() const {
    return user_id;
}

void ContextManager::set_user_id(int new_id) {
    user_id = new_id;
}
