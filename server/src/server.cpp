#include "server.h"
#include "game.h"
#include <boost/log/trivial.hpp>
#include <iostream>
#include <memory>
#include <optional>
#include <thread>

namespace FastTyping::Server {
    Server::Server() : acceptor(ioContext, tcp::endpoint(tcp::v4(), PORT)), userStorage(new DBUserStorage), gameStorage(new MapGameStorage) {
        std::cout << "Listening at " << acceptor.local_endpoint() << std::endl;
        commonQueriesMap["echo"] = [&](const json &body, int user_id) {
            return body;
        };
        commonQueriesMap["createGame"] = [&](const json &body, int user_id) -> json {
            // basic checks
            if (!body.contains("dictionaryName") || !body["dictionaryName"].is_string()) {
                return {{"header", {{"type", "error"}}}, {"body", {{"text", "can't find \"dictionaryName\""}}}};
            }

            if (!body.contains("parserName") || !body["parserName"].is_string()) {
                return {{"header", {{"type", "error"}}}, {"body", {{"text", "can't find \"parserName\""}}}};
            }

            if (gameStorage->getGame(userStorage->getGameId(user_id)) != nullptr) {
                return {{"header", {{"type", "error"}}}, {"body", {{"text", "Already in game"}}}};
            }

            auto result = gameStorage->createGame(body);
            if (body.contains("autoJoin") && body["autoJoin"].is_boolean() && body["autoJoin"]) {
                json errors;
                auto game = gameStorage->get(result["body"]["id"], errors);
                if (game == nullptr) {
                    result["joined"] = false;
                    result["error"] = errors;
                } else {
                    gameStorage->setGame(userStorage->getGameId(user_id), game);
                    result["joined"] = true;
                    result["error"] = "";
                }
            }
            return result;
        };
        commonQueriesMap["joinGame"] = [&](const json &body, int user_id) -> json {
            if (!body.contains("id") || !body["id"].is_number_unsigned()) {
                return {{"header", {{"type", "error"}}}, {"body", {{"text", "can't find \"id\""}}}};
            }

            if (gameStorage->getGame(userStorage->getGameId(user_id)) != nullptr) {
                return {{"header", {{"type", "error"}}}, {"body", {{"text", "Already in game"}}}};
            }
            json errors;
            auto game = gameStorage->get(body["id"], errors);
            if (game == nullptr) {
                return errors;
            }
            if (game->hasUser(user_id)) {
                return {{"header", {{"type", "error"}}}, {"body", {{"text", "Try connect after disconnect"}}}};
            }
          gameStorage->setGame(userStorage->getGameId(user_id), game);

          return {{"header", {{"type", "GameJoinedSuccessfully"}}}, {"body", {{"id", game->getId()}}}};
        };

        commonQueriesMap["leaveGame"] = [&](const json &body, int user_id) -> json {
            if (!body.contains("id") || !body["id"].is_number_unsigned()) {
                return {{"header", {{"type", "error"}}}, {"body", {{"text", "can't find \"id\""}}}};
            }

            if (gameStorage->getGame(userStorage->getGameId(user_id)) == nullptr) {
                return {{"header", {{"type", "error"}}}, {"body", {{"text", "Not in game"}}}};
            }

          gameStorage->setGame(userStorage->getGameId(user_id), nullptr);

          return {{"header", {{"type", "GameLeaveSuccessfully"}}}, {"body", {}}};
        };

        commonQueriesMap["getNewLine"] = [&](const json &body, int user_id) -> json {
            if (gameStorage->getGame(userStorage->getGameId(user_id)) == nullptr) {
                return {{"header", {{"type", "error"}}}, {"body", {{"text", "not in game"}}}};
            }
            return (gameStorage->getGame(userStorage->getGameId(user_id)))->getNewLine(user_id);
        };

        commonQueriesMap["addNewChar"] = [&](const json &body, int user_id) -> json {
            if (gameStorage->getGame(userStorage->getGameId(user_id)) == nullptr) {
                return {{"header", {{"type", "error"}}}, {"body", {{"text", "not in game"}}}};
            }
            if (!body.contains("char") || !body["char"].is_string() || body["char"].get<std::string>().size() != 1) {
                return {{"header", {{"type", "error"}}}, {"body", {{"text", "can't find word to check"}}}};
            }

            auto result = (gameStorage->getGame(userStorage->getGameId(user_id)))->addNewChar(user_id, body["char"].get<std::string>()[0]);
            if (result["body"]["isFullCorrect"] == true && result["body"]["isEnd"] == true) {
                gameStorage->setGame(userStorage->getGameId(user_id), nullptr);
            }
            return result;
        };

        commonQueriesMap["backspace"] = [&](const json &body, int user_id) -> json {
            if (gameStorage->getGame(userStorage->getGameId(user_id))  == nullptr) {
                return {{"header", {{"type", "error"}}}, {"body", {{"text", "not in game"}}}};
            }
            return (gameStorage->getGame(userStorage->getGameId(user_id)))->backspace(user_id);
        };

        commonQueriesMap["getStates"] = [&](const json &body, int user_id) -> json {
            if (gameStorage->getGame(userStorage->getGameId(user_id)) == nullptr) {
                return {{"header", {{"type", "error"}}}, {"body", {{"text", "not in game"}}}};
            }
            return (gameStorage->getGame(userStorage->getGameId(user_id)))->getStateOfUsers();
        };

        commonQueriesMap["getNewWord"] = [&](const json &body, int user_id) -> json {
            if (gameStorage->getGame(userStorage->getGameId(user_id)) == nullptr) {
                return {{"header", {{"type", "error"}}}, {"body", {{"text", "not in game"}}}};
            }
            return (gameStorage->getGame(userStorage->getGameId(user_id)))->getNewWord(user_id);
        };

        commonQueriesMap["exit"] = [&](const json &body, int user_id) -> json {
            userStorage->setWantExit(user_id);
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

        int user_id = userStorage->getId(user_name);
        json result = {{"header", {{"type", "loginSuccessfully"}}}, {"body", {{"name", userStorage->getName(user_id)}}}};
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
                if (auto it = commonQueriesMap.find(queryHeader["type"]); it != commonQueriesMap.end()) {
                    result = it->second(queryBody, user_id);
                    if (userStorage->getWantExit(user_id)) {
                        userStorage->unsetWantExit(user_id);
                        break;
                    }
                    client << result << '\n';
                }
            }
        } catch (nlohmann::detail::exception &e) {
            std::cerr << e.what() << std::endl; // process error to client
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