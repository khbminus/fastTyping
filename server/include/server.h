#ifndef FASTTYPING_SERVER_H
#define FASTTYPING_SERVER_H
#include <boost/asio.hpp>
#include <deque>
#include <memory>
#include <optional>
#include <string>
#include "database.h"
#include "game.h"
#include "user.h"
namespace FastTyping::Server {
using boost::asio::ip::tcp;

struct Connection;

struct Server {
public:
    static Server &getInstance(boost::asio::io_context &ioContext) {
        static Server server(ioContext);
        return server;
    }
    void polling();

private:
    static inline const int PORT = 1337;

    Server(boost::asio::io_context &ioContext);
    boost::asio::io_context &ioContext;
    tcp::acceptor acceptor;
    std::unique_ptr<Database> userStorage;
    std::unique_ptr<AbstractGameStorage> gameStorage;
    std::unordered_map<std::string, std::function<json(const json &, User &)>>
        commonQueriesMap;
    std::unordered_map<std::string, std::function<json(const json &)>>
        loginQueriesMap;
    void parseQuery(tcp::socket);

    void acceptHandler(std::shared_ptr<Connection> conn,
                       boost::system::error_code e);

    friend struct Connection;
    std::list<std::shared_ptr<Connection>> connections;
};

struct Connection {
    Connection(boost::asio::io_context &context, Server &server)
        : server(server), mSocket(context), context(context) {}
    tcp::socket &socket() {
        return mSocket;
    }
    explicit operator bool() {
        return !wasDisconnect;
    }

    void readLine();
    void writeLine(std::string);

private:
    Server &server;

    boost::asio::streambuf readBuf;
    std::deque<json> readQueue;
    bool wasDisconnect;

    std::deque<std::string> writeQueue;

    tcp::socket mSocket;
    boost::asio::io_context &context;

    std::optional<User> user;

    void handleRead();
    void handleLogin(const json &query);
    void handleCommonQuery(const json &query);

    void doWrite(boost::system::error_code e);
    void doRead(boost::system::error_code e, std::size_t btx);
};

std::optional<json> checkQueryCorrectness(const std::string &queryString);

}  // namespace FastTyping::Server

#endif  // FASTTYPING_SERVER_H
