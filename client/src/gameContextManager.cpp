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

WebManager *ContextManager::get_remote_manager() const {
    return remote_manager.get();
}

void ContextManager::set_context(const std::vector<QString> &words,
                                 bool isSolo) {
    using client::responses::APIHandler;
    using client::responses::handler;
    if (!isSolo) {
        local_manager.reset(new LocalManager(words));

    } else {
        local_manager.reset(new LocalManagerSolo(words));
    }
    remote_manager.reset(new WebManager(words));  // TODO make solo??



    QObject::connect(&handler(), &APIHandler::correct_signal,
                     remote_manager.get(), &WebManager::correct_slot);
    QObject::connect(&handler(), &APIHandler::error_signal,
                     remote_manager.get(), &WebManager::error_slot);
    QObject::connect(&handler(), &APIHandler::end_signal, remote_manager.get(),
                     &WebManager::end_slot);
    QObject::connect(&handler(), &APIHandler::correct_word_signal,
                     remote_manager.get(), &WebManager::correct_word_slot);

    auto &controller = FastTyping::WindowController::getInstance();
    gameWindow.reset(new GameWindow({local_manager.get(), remote_manager.get()},
                                    local_manager.get()));
    controller.registerWindow("GameWindow", gameWindow.get());
}

void ContextManager::set_context_from_create_query(json const &response,
                                                   bool isSolo) {
    game_id = response["body"]["id"].get<int>();
    std::vector<QString> words = get_line();
    set_context(words, isSolo);
}

void ContextManager::reset_context() {
    local_manager.clear();
    remote_manager.clear();
    auto &controller = FastTyping::WindowController::getInstance();
    controller.dropWindow("GameWindow");
    gameWindow.reset();
    controller.dropWindow("StatWindow");
    statisticsWindow.reset();
}
void ContextManager::createStatisticsWindow() {
    statisticsWindow.reset(new StatWindow(local_manager.get()));
    auto &controller = FastTyping::WindowController::getInstance();
    controller.registerWindow("StatWindow", statisticsWindow.get());
}