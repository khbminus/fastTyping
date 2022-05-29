#include "gameContextManager.h"
#include <algorithm>
#include "queryTemplates.h"
#include "responseHandler.h"
#include "sonicSocket.h"
#include "windowcontroller.h"

using nlohmann::json;

std::vector<QString> get_line() {
    using client::queries::get_line_query;
    using client::web::socket;

    json line = json::parse(socket().query(get_line_query()).toStdString());
    std::vector<std::string> raw_words =
        line["body"]["line"].get<std::vector<std::string>>();
    std::vector<QString> words;
    std::transform(raw_words.begin(), raw_words.end(),
                   std::back_inserter(words), QString::fromStdString);
    return words;
}

int ContextManager::get_user_id() const {
    return user_id;
}

int ContextManager::get_game_id() const {
    return game_id;
}

void ContextManager::set_user_id(int new_id) {
    user_id = new_id;
}

LocalManager *ContextManager::get_local_manager() const {
    return local_manager.get();
}

LocalManager *ContextManager::get_remote_manager() const {
    return local_manager.get();
}

void ContextManager::set_context(const std::vector<QString> &words) {
    using client::responses::APIHandler;
    using client::responses::handler;

    local_manager.reset(new LocalManager(words));
    remote_manager.reset(new WebManager(words));

    QObject::connect(&handler(), &APIHandler::correct_signal,
                     remote_manager.get(), &WebManager::correct_slot);
    QObject::connect(&handler(), &APIHandler::error_signal,
                     remote_manager.get(), &WebManager::error_slot);
    QObject::connect(&handler(), &APIHandler::end_signal, remote_manager.get(),
                     &WebManager::end_slot);
    QObject::connect(&handler(), &APIHandler::correct_word_signal,
                     remote_manager.get(), &WebManager::correct_word_slot);

    auto &controller = FastTyping::WindowController::getInstance();
    window = new GameWindow({local_manager.get(), remote_manager.get()},
                            local_manager.get());
    controller.registerWindow("GameWindow", window);
}

void ContextManager::set_context_from_create_query(json const &response) {
    game_id = response["body"]["id"].get<int>();
    std::vector<QString> words = get_line();
    set_context(words);
}

void ContextManager::set_context_from_join_query(json const &response) {
    game_id = response["body"]["id"].get<int>();
    std::vector<QString> words = get_line();
    set_context(words);
}

void ContextManager::reset_context() {
    local_manager.clear();
    remote_manager.clear();
    auto &controller = FastTyping::WindowController::getInstance();
    controller.dropWindow("GameWindow");
    window = nullptr;
}
