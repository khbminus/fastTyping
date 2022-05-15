#ifndef GAME_CONTEXT_MANAGER
#define GAME_CONTEXT_MANAGER

#include <QSharedPointer>
#include <nlohmann/json.hpp>
#include "gameManager.h"
#include "localManager.h"
#include "sonicSocket.h"
#include "webManager.h"

class ContextManager {
    int user_id;
    ContextManager() = default;
    QSharedPointer<WebManager> remote_manager;
    QSharedPointer<LocalManager> local_manager;

public:
    ContextManager &get_instance() {
        static ContextManager instance;
        return instance;
    }
    void set_user_id(int new_id);
    int get_user_id() const;
    void set_context_from_create_query(nlohmann::json const &create_query);
    void set_context_from_join_query(nlohmann::json const &join_query);
};

#endif
