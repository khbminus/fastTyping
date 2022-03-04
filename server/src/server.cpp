#include "server.h"
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

        try {
            while (client) {
                std::string line;
                if (!std::getline(client, line)) {
                    break;
                }
                json queryHeader = json::parse(line);

                if (!std::getline(client, line)) {
                    break;
                }

                json queryBody = json::parse(line);

                std::string type = queryHeader["type"].get<std::string>();
                if (type == "exit") {
                    break;
                }
                if (type == "echo") {
                    client << queryBody << '\n';
                }
            }
        } catch (nlohmann::detail::parse_error& e) {
            std::cerr << e.what() << std::endl; // process error to client
        }

        std::cout << "Disconnected: " << client.socket().remote_endpoint() << "->" << client.socket().local_endpoint() << std::endl;
    }
}// namespace FastTyping::Server