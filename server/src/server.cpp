#include "server.h"
#include "game.h"
#include <iostream>
#include <memory>
#include <optional>
#include <thread>

namespace FastTyping::Server {
    Server::Server() : acceptor(ioContext, tcp::endpoint(tcp::v4(), PORT)), storage(new MapUserStorage) {
        std::cout << "Listening at " << acceptor.local_endpoint() << std::endl;
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
        auto errors = checkQueryForErrors(line);
        if (errors) {
            client << errors.value() << '\n';
            return;
        }
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
        User &user = storage->get(query["body"]["name"].get<std::string>());
        std::unique_ptr<Game> currentGame = nullptr;
        try {
            while (client) {
                if (!std::getline(client, line)) {
                    break;
                }
                errors = checkQueryForErrors(line);
                if (errors) {
                    client << errors.value() << '\n';
                    continue;
                }
                query = json::parse(line);
                auto queryHeader = query["header"];
                auto queryBody = query["body"];

                auto type = queryHeader["type"].get<std::string>();
                if (type == "exit") {
                    break;
                } else if (currentGame) {
                    if (type == "getNewLine") {
                        client << currentGame->getNewLine(user, queryBody) << '\n';
                    } else if (type == "checkInput") {
                        auto result = currentGame->checkInputAndProceed(user, queryBody);
                        client << result << '\n';
                        if (result["body"]["isCorrect"] == true && result["body"]["isEnd"] == true) {
                            currentGame = nullptr;
                        }
                    } else {
                        json result = {{"header", {{"type", "error"}}}, {"body", {{"text", "unknown query"}}}};
                        client << result << '\n';
                    }
                } else if (type == "echo") {
                    echoQuery(client, user, queryBody);
                } else if (type == "createGame") {
                    json errors;
                    currentGame = makeGame(user, queryBody, errors);
                    if (currentGame == nullptr) {
                        client << errors << '\n';
                    } else {
                        json result = {{"header", {{"type", "gameCreated"}}}, {"body", {{"id", currentGame->getId()}, {"name", currentGame->getName()}}}};
                        client << result << '\n';
                    }
                } else {
                    json result = {{"header", {{"type", "error"}}}, {"body", {{"text", "unknown query"}}}};
                    client << result << '\n';
                }
            }
        } catch (nlohmann::detail::parse_error &e) {
            std::cerr << e.what() << std::endl;// process error to client
        }

        std::cout << "Disconnected: " << client.socket().remote_endpoint() << "->" << client.socket().local_endpoint() << std::endl;
    }
    void Server::echoQuery(tcp::iostream &client, User &user, json queryBody) {
        client << queryBody << '\n';
    }
    std::optional<json> Server::checkQueryForErrors(const std::string &queryString) {
        json query;
        try {
            query = json::parse(queryString);
        } catch (nlohmann::detail::parse_error &e) {
            return {{{"header", {{"type", "error"}}}, {"body", {{"text", "bad json"}}}}};
        }
        if (!query.contains("header") || !query["header"].is_object()) {
            return {{{"header", {{"type", "error"}}}, {"body", {{"text", "can't find header"}}}}};
        }
        if (!query.contains("body") || !query["body"].is_object()) {
            return {{{"header", {{"type", "error"}}}, {"body", {{"text", "can't find body"}}}}};
        }

        if (!query["header"]["type"].is_string()) {
            return {{{"header", {{"type", "error"}}}, {"body", {{"text", "can't recognize type of query"}}}}};
        }

        return {};
    }
}// namespace FastTyping::Server