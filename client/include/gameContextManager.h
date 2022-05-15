#ifndef GAME_CONTEXT_MANAGER
#define GAME_CONTEXT_MANAGER

class ContextManager {
    int user_id;
    ContextManager() = default;

public:
    ContextManager &get_instance() {
        static ContextManager instance;
        return instance;
    }
    void set_user_id(int new_id);
    int get_user_id() const;
};

#endif
