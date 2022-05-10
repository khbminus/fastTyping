#include "server.h"
#include <boost/log/trivial.hpp>
#include <iostream>
#include <memory>
#include <optional>
#include <thread>
#include "game.h"

namespace FastTyping::Server {
Server::Server()
    : acceptor(ioContext, tcp::endpoint(tcp::v4(), PORT)),
      userStorage(new MapUserStorage),
      gameStorage(new MapGameStorage) {
    std::cout << "Listening at " << acceptor.local_endpoint() << std::endl;
    commonQueriesMap["echo"] = [&](const json &body, User &user) {
        return body;
    };
    commonQueriesMap["createGame"] = [&](const json &body, User &user) -> json {
        // basic checks
        if (!body.contains("dictionaryName") ||
            !body["dictionaryName"].is_string()) {
            return {{"header", {{"type", "wrongFormatError"}}},
                    {"body", {{"text", "can't find \"dictionaryName\""}}}};
        }

        if (!body.contains("parserName") || !body["parserName"].is_string()) {
            return {{"header", {{"type", "wrongFormatError"}}},
                    {"body", {{"text", "can't find \"parserName\""}}}};
        }

        if (user.getGame() != nullptr) {
            return {{"header", {{"type", "alreadyInGameError"}}},
                    {"body", {{"text", "Already in game"}}}};
        }

        auto result = gameStorage->createGame(body);
        if (body.contains("autoJoin") && body["autoJoin"].is_boolean() &&
            body["autoJoin"]) {
            json errors;
            auto game = gameStorage->get(result["body"]["id"], errors);
            if (game == nullptr) {
                result["joined"] = false;
                result["error"] = errors;
            } else {
                user.setGame(game);
                result["joined"] = true;
                result["error"] = "";
            }
        }
        return result;
    };
    commonQueriesMap["joinGame"] = [&](const json &body, User &user) -> json {
        if (!body.contains("id") || !body["id"].is_number_unsigned()) {
            return {{"header", {{"type", "wrongFormatError"}}},
                    {"body", {{"text", "can't find \"id\""}}}};
        }

        if (user.getGame() != nullptr) {
            return {{"header", {{"type", "alreadyInGameError"}}},
                    {"body", {{"text", "Already in game"}}}};
        }
        json errors;
        auto game = gameStorage->get(body["id"], errors);
        if (game == nullptr) {
            return errors;
        }
        if (game->hasUser(user.getId())) {
            return {{"header", {{"type", "connectAfterLeaveError"}}},
                    {"body", {{"text", "Try connect after disconnect"}}}};
        }
        user.setGame(game);
        return {{"header", {{"type", "GameJoinedSuccessfully"}}},
                {"body", {{"id", game->getId()}}}};
    };
    commonQueriesMap["leaveGame"] = [&](const json &body, User &user) -> json {
        if (user.getGame() == nullptr) {
            return {{"header", {{"type", "notInGameError"}}},
                    {"body", {{"text", "Not in game"}}}};
        }

        user.setGame(nullptr);
        return {{"header", {{"type", "GameLeaveSuccessfully"}}}, {"body", {}}};
    };
    commonQueriesMap["getNewLine"] = [&](const json &body, User &user) -> json {
        if (user.getGame() == nullptr) {
            return {{"header", {{"type", "notInGameError"}}},
                    {"body", {{"text", "not in game"}}}};
        }
        return user.getGame()->getNewLine(user.getId());
    };
    commonQueriesMap["addNewChar"] = [&](const json &body, User &user) -> json {
        if (user.getGame() == nullptr) {
            return {{"header", {{"type", "notInGameError"}}},
                    {"body", {{"text", "not in game"}}}};
        }
        if (!body.contains("char") || !body["char"].is_string() ||
            body["char"].get<std::string>().size() != 1) {
            return {{"header", {{"type", "wrongFormatError"}}},
                    {"body", {{"text", "can't find word to check"}}}};
        }

        auto result = user.getGame()->addNewChar(
            user.getId(), body["char"].get<std::string>()[0]);
        return result;
    };
    commonQueriesMap["backspace"] = [&](const json &body, User &user) -> json {
        if (user.getGame() == nullptr) {
            return {{"header", {{"type", "notInGameError"}}},
                    {"body", {{"text", "not in game"}}}};
        }
        return user.getGame()->backspace(user.getId());
    };
    commonQueriesMap["getStates"] = [&](const json &body, User &user) -> json {
        if (user.getGame() == nullptr) {
            return {{"header", {{"type", "notInGameError"}}},
                    {"body", {{"text", "not in game"}}}};
        }
        return user.getGame()->getStateOfUsers();
    };
    commonQueriesMap["getNewWord"] = [&](const json &body, User &user) -> json {
        if (user.getGame() == nullptr) {
            return {{"header", {{"type", "notInGameError"}}},
                    {"body", {{"text", "not in game"}}}};
        }
        return user.getGame()->getNewWord(user.getId());
    };
    commonQueriesMap["clearBuffer"] = [&](const json &body,
                                          User &user) -> json {
        if (user.getGame() == nullptr) {
            return {{"header", {{"type", "notInGameError"}}},
                    {"body", {{"text", "not in game"}}}};
        }
        return user.getGame()->clearBuffer(user.getId());
    };
    commonQueriesMap["exit"] = [&](const json &body, User &user) -> json {
        user.setWillToExit();
        return {};
    };
}

[[noreturn]] void Server::polling() {
    while (true) {
        tcp::socket sock = acceptor.accept();
        std::thread([s = std::move(sock), this]() mutable {
            parseQuery(std::move(s));
        }).detach();
    }
}

void Server::parseQuery(tcp::socket s) {
    try {
        tcp::iostream client(std::move(s));
        std::cout << "New client: " << client.socket().remote_endpoint() << "->"
                  << client.socket().local_endpoint() << std::endl;
        std::string line;
        if (!std::getline(client, line)) {
            std::cout << "Disconnected: " << client.socket().remote_endpoint()
                      << "->" << client.socket().local_endpoint() << std::endl;
            return;
        }
        json query;
        std::string user_name;
        auto errors = checkQueryCorrectness(line);
        if (errors) {
            client << errors.value() << '\n';
            return;
        }
        try {
            query = json::parse(line);
            if (query["header"]["type"] != "hello") {
                json result = {
                    {"header", {{"type", "wrongFirstQueryError"}}},
                    {"body", {{"text", "first query should be hello"}}}};
                client << result << '\n';
                return;
            }
            if (!query["body"]["name"].is_string()) {
                json result = {{"header", {{"type", "wrongFormatError"}}},
                               {"body", {{"text", "can't find \"name\""}}}};
                client << result << '\n';
            }
            user_name = query["body"]["name"].get<std::string>();
        } catch (nlohmann::detail::exception &e) {
            std::cerr << e.what() << std::endl;  // process error to client
            return;
        }

        User &user = userStorage->get(user_name);
        json result = {{"header", {{"type", "loginSuccessfully"}}},
                       {"body", {{"name", user.name()}}}};
        result["header"]["queryType"] = query["header"]["type"];
        client << result << '\n';

        try {
            while (client) {
                if (!std::getline(client, line)) {
                    break;
                }
                errors = checkQueryCorrectness(line);
                if (errors) {
                    client << errors.value() << '\n';
                    continue;
                }
                query = json::parse(line);
                auto queryHeader = query["header"];
                auto queryBody = query["body"];
                BOOST_LOG_TRIVIAL(debug) << query << '\n';
                if (auto it = commonQueriesMap.find(queryHeader["type"]);
                    it != commonQueriesMap.end()) {
                    result = it->second(queryBody, user);
                    result["header"]["queryType"] = queryHeader["type"];
                    if (user.isWantToExit()) {
                        user.clearWillToExit();
                        break;
                    }
                    client << result << '\n';
                }
            }
        } catch (nlohmann::detail::exception &e) {
            std::cerr << e.what() << std::endl;  // process error to client
        }
        user.setGame(nullptr);
        std::cout << "Disconnected: " << client.socket().remote_endpoint()
                  << "->" << client.socket().local_endpoint() << std::endl;
    } catch (boost::system::system_error &e) {
        std::cerr << "Unknown Boost::ASIO error occurred: " << e.what() << '\n';
    }
}
std::optional<json> Server::checkQueryCorrectness(
    const std::string &queryString) {
    json query;
    try {
        query = json::parse(queryString);
    } catch (nlohmann::detail::exception &e) {
        return {{{"header", {{"type", "wrongFormatError"}}},
                 {"body", {{"text", "bad json"}}}}};
    }
    if (!query.contains("header") || !query["header"].is_object()) {
        return {{{"header", {{"type", "wrongFormatError"}}},
                 {"body", {{"text", "can't find header"}}}}};
    }
    if (!query.contains("body") ||
        (!query["body"].is_null() && !query["body"].is_object())) {
        return {{{"header", {{"type", "wrongFormatError"}}},
                 {"body", {{"text", "can't find body"}}}}};
    }

    if (!query["header"].contains("type") ||
        !query["header"]["type"].is_string()) {
        return {{{"header", {{"type", "wrongFormatError"}}},
                 {"body", {{"text", "can't recognize type of query"}}}}};
    }

    return {};
}
}  // namespace FastTyping::Server