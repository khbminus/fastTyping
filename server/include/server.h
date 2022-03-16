#ifndef FASTTYPING_SERVER_H
#define FASTTYPING_SERVER_H
#include "user.h"
#include <boost/asio.hpp>
#include <memory>
#include <string>
namespace FastTyping::Server {
    using boost::asio::ip::tcp;

    struct Server {
    public:
        static Server &getInstance() {
            static Server server;
            return server;
        }
        [[noreturn]] void polling();

    private:
        static inline const int PORT = 1337;

        Server();
        boost::asio::io_context ioContext;
        tcp::acceptor acceptor;
        std::unique_ptr<AbstractUserStorage> storage;

        void parseQuery(tcp::socket);
        void echoQuery(tcp::iostream &client, User &user, json queryBody);
    };
}// namespace FastTyping::Server

#endif//FASTTYPING_SERVER_H
