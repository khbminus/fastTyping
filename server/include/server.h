#ifndef FASTTYPING_SERVER_H
#define FASTTYPING_SERVER_H
#include <boost/asio.hpp>
#include <memory>
#include <optional>
#include <string>
#include "database.h"
#include "game.h"
#include "statisticsDB.h"
#include "user.h"

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
    std::unique_ptr<UserStorage> user_storage;
    std::unique_ptr<DictionariesStorage> dictionaries_storage;
    std::unique_ptr<MistakesStorage> mistakes_storage;
    std::unique_ptr<AbstractGameStorage> gameStorage;
    std::unique_ptr<StatisticsStorage> statisticsStorage;
    std::unordered_map<std::string, std::function<json(const json &, User &)>>
        commonQueriesMap;
    std::unordered_map<std::string, std::function<json(const json &)>>
        loginQueriesMap;
    void parseQuery(tcp::socket);

    std::optional<json> checkQueryCorrectness(const std::string &queryString);
};

}  // namespace FastTyping::Server

#endif  // FASTTYPING_SERVER_H
