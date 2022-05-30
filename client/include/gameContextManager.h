#ifndef GAME_CONTEXT_MANAGER
#define GAME_CONTEXT_MANAGER

#include <QSharedPointer>
#include <nlohmann/json.hpp>
#include "gameManager.h"
#include "gamewindow.h"
#include "localManager.h"
#include "sonicSocket.h"
#include "statwindow.h"
#include "webManager.h"

class ContextManager {
    int user_id;
    int game_id;
    ContextManager() = default;
    QSharedPointer<WebManager> remote_manager;
    QSharedPointer<LocalManager> local_manager;
    GameWindow *gameWindow;
    StatWindow *statisticsWindow;
    void set_context(std::vector<QString> const &words);

public:
    static ContextManager &get_instance() {
        static ContextManager instance;
        return instance;
    }
    void set_user_id(int new_id);
    int get_user_id() const;
    int get_game_id() const;
    LocalManager *get_local_manager() const;
    LocalManager *get_remote_manager() const;
    void set_context_from_create_query(nlohmann::json const &create_query);
    void set_context_from_join_query(nlohmann::json const &join_query);
    void reset_context();
    void createStatisticsWindow();
};

#endif
