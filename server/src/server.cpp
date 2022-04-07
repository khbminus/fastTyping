#include "server.h"
#include "game.h"
#include <iostream>
#include <memory>
#include <optional>
#include <thread>

namespace FastTyping::Server {
    Server::Server() : acceptor(ioContext, tcp::endpoint(tcp::v4(), PORT)), userStorage(new MapUserStorage), gameStorage(new MapGameStorage) {
        std::cout << "Listening at " << acceptor.local_endpoint() << std::endl;
        commonQueriesMap["echo"] = [&](const json &body, User &user) {
            return body;
        };
        commonQueriesMap["createGame"] = [&](const json &body, User &user) -> json {
            // basic checks
            if (!body.contains("dictionaryName") || !body["dictionaryName"].is_string()) {
                return {{"header", {{"type", "error"}}}, {"body", {{"text", "can't find \"dictionaryName\""}}}};
            }

            if (!body.contains("parserName") || !body["parserName"].is_string()) {
                return {{"header", {{"type", "error"}}}, {"body", {{"text", "can't find \"parserName\""}}}};
            }
            return gameStorage->createGame(body);
        };
        commonQueriesMap["joinGame"] = [&](const json &body, User &user) -> json {
            if (!body.contains("id") || !body["id"].is_number_unsigned()) {
                return {{"header", {{"type", "error"}}}, {"body", {{"text", "can't find \"id\""}}}};
            }
            json errors;
            auto game = gameStorage->get(body["id"], errors);
            if (game == nullptr) {
                return errors;
            }
            user.setGame(game);
            return {{"header", {{"type", "GameJoinedSuccessfully"}}}, {"body", {{"id", game->getId()}}}};
        };

        commonQueriesMap["getNewLine"] = [&](const json &body, User &user) -> json {
            if (user.getGame() == nullptr) {
                return {{"header", {{"type", "error"}}}, {"body", {{"text", "not in game"}}}};
            }
            return user.getGame()->getNewLine(user.getId());
        };

        commonQueriesMap["addNewChar"] = [&](const json &body, User &user) -> json {
            if (user.getGame() == nullptr) {
                return {{"header", {{"type", "error"}}}, {"body", {{"text", "not in game"}}}};
            }
            if (!body.contains("char") || !body["char"].is_string() || body["char"].get<std::string>().size() != 1) {
                return {{"header", {{"type", "error"}}}, {"body", {{"text", "can't find word to check"}}}};
            }

            auto result = user.getGame()->addNewChar(user.getId(), body["char"].get<std::string>()[0]);
            if (result["body"]["isFullCorrect"] == true && result["body"]["isEnd"] == true) {
                user.setGame(nullptr);
            }
            return result;
        };

        commonQueriesMap["backspace"] = [&](const json &body, User &user) -> json {
            if (user.getGame() == nullptr) {
                return {{"header", {{"type", "error"}}}, {"body", {{"text", "not in game"}}}};
            }
            return user.getGame()->backspace(user.getId());
        };

        commonQueriesMap["getStates"] = [&](const json &body, User &user) -> json {
            if (user.getGame() == nullptr) {
                return {{"header", {{"type", "error"}}}, {"body", {{"text", "not in game"}}}};
            }
            return user.getGame()->getStateOfUsers();
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
        tcp::iostream client(std::move(s));
        std::cout << "New client: " << client.socket().remote_endpoint() << "->" << client.socket().local_endpoint() << std::endl;
        std::string line;
        if (!std::getline(client, line)) {
            std::cout << "Disconnected: " << client.socket().remote_endpoint() << "->" << client.socket().local_endpoint() << std::endl;
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
                json result = {{"header", {{"type", "error"}}}, {"body", {{"text", "first query should be hello"}}}};
                client << result << '\n';
                return;
            }
            if (!query["body"]["name"].is_string()) {
                json result = {{"header", {{"type", "error"}}}, {"body", {{"text", "can't find \"name\""}}}};
                client << result << '\n';
            }
            user_name = query["body"]["name"].get<std::string>();
        } catch (nlohmann::detail::exception &e) {
            std::cerr << e.what() << std::endl;// process error to client
            return;
        }

        User &user = userStorage->get(user_name);
        json result = {{"header", {{"type", "loginSuccessfully"}}}, {"body", {{"name", user.name()}}}};
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
                if (auto it = commonQueriesMap.find(queryHeader["type"]); it != commonQueriesMap.end()) {
                    result = it->second(queryBody, user);
                    if (user.isWantToExit()) {
                        user.clearWillToExit();
                        break;
                    }
                    client << result << '\n';
                }
            }
        } catch (nlohmann::detail::exception &e) {
            std::cerr << e.what() << std::endl;// process error to client
        }

        std::cout << "Disconnected: " << client.socket().remote_endpoint() << "->" << client.socket().local_endpoint() << std::endl;
    }
    std::optional<json> Server::checkQueryCorrectness(const std::string &queryString) {
        json query;
        try {
            query = json::parse(queryString);
        } catch (nlohmann::detail::exception &e) {
            return {{{"header", {{"type", "error"}}}, {"body", {{"text", "bad json"}}}}};
        }
        if (!query.contains("header") || !query["header"].is_object()) {
            return {{{"header", {{"type", "error"}}}, {"body", {{"text", "can't find header"}}}}};
        }
        if (!query.contains("body") || !query["body"].is_object()) {
            return {{{"header", {{"type", "error"}}}, {"body", {{"text", "can't find body"}}}}};
        }

        if (!query["header"].contains("type") || !query["header"]["type"].is_string()) {
            return {{{"header", {{"type", "error"}}}, {"body", {{"text", "can't recognize type of query"}}}}};
        }

        return {};
    }
}// namespace FastTyping::Server