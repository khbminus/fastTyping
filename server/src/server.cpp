#include "server.h"
#include "game.h"
#include <iostream>
#include <memory>
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
        try {
            query = json::parse(line);
            if (query["header"]["type"] != "hello") {
                client << "BAD QUERY\n";
                return;
            }
            user_name = query["body"]["name"].get<std::string>();

        } catch (nlohmann::detail::parse_error &e) {
            std::cerr << e.what() << '\n';
            return;
        }

        User &user = storage->get(user_name);
        std::unique_ptr<Game> currentGame = nullptr;
        try {
            while (client) {
                if (!std::getline(client, line)) {
                    break;
                }
                query = json::parse(line);
                if (!query.contains("header")) {
                    client << "ERROR\n";// FIXME: make it better;
                    continue;
                }
                if (!query.contains("body")) {
                    client << "ANOTHER ERROR\n";// FIXME
                    continue;
                }
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
                        client << "unknown command\n";
                    }
                } else if (type == "echo") {
                    echoQuery(client, user, queryBody);
                } else if (type == "createGame") {
                    currentGame = makeGame(user, queryBody);
                    json result = {{"header", {{"type", "gameCreated"}}}, {"body", {{"id", currentGame->getId()}, {"name", currentGame->getName()}}}};
                    client << result << '\n';
                } else {
                    client << "unknown command\n";
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
}// namespace FastTyping::Server