#include "server.h"
#include <boost/log/trivial.hpp>
#include <iostream>
#include <memory>
#include <optional>
#include <thread>
#include "game.h"

namespace {}

namespace FastTyping::Server {
Server::Server(boost::asio::io_context &ioContext)
    : ioContext(ioContext),
      acceptor(ioContext, tcp::endpoint(tcp::v4(), PORT)),
      userStorage(new Database),
      gameStorage(new MapGameStorage) {
    std::cout << "Listening at " << acceptor.local_endpoint() << std::endl;
    commonQueriesMap["echo"] = [&](const json &body, User &user) -> json {
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

        auto result = gameStorage->createGame(body, user.getId());
        if (body.contains("autoJoin") && body["autoJoin"].is_boolean() &&
            body["autoJoin"]) {
            if (!result["body"].contains("id") ||
                !result["body"]["id"].is_number()) {
                result["body"]["joined"] = false;
                return result;
            }
            json errors;
            auto game = gameStorage->get(result["body"]["id"], errors);
            if (game == nullptr) {
                result["body"]["joined"] = false;
                result["body"]["error"] = errors;
            } else {
                user.setGame(game);
                result["body"]["joined"] = true;
                result["body"]["error"] = "";
            }
        }
        return result;
    };
    //{"body":{"id":0},"header":{"type":"joinGame"}}
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
        if (game->getGameStarted()) {
            return {{"header", {{"type", "connectAfterStartError"}}},
                    {"body", {{"text", "Try connect after game started"}}}};
        }
        if (game->hasUser(user.getId())) {
            return {{"header", {{"type", "connectAfterLeaveError"}}},
                    {"body", {{"text", "Try connect after disconnect"}}}};
        }
        user.setGame(game);
        return {{"header", {{"type", "GameJoinedSuccessfully"}}},
                {"body", {{"id", game->getId()}}}};
    };
    commonQueriesMap["waitGameStart"] = [&](const json &body,
                                            User &user) -> json {
        if (user.getGame() == nullptr) {
            return {{"header", {{"type", "notInGameError"}}},
                    {"body", {{"text", "Not in game"}}}};
        }
        json errors;
        auto game = user.getGame();
        if (game == nullptr) {
            return errors;
        }
        if (game->getHostId() == user.getId()) {
            return {{"header", {{"type", "UserHostError"}}},
                    {"body",
                     {{"text", "User is a host of the game, he can't wait"}}}};
        }
        user.waitStartGame();
        return {{"header", {{"type", "GameWaitedSuccessfully"}}}, {"body", {}}};
    };

    commonQueriesMap["startGame"] = [&](const json &body, User &user) -> json {
        if (user.getGame() == nullptr) {
            return {{"header", {{"type", "notInGameError"}}},
                    {"body", {{"text", "Not in game"}}}};
        }
        if (user.getGame()->getGameStarted()) {
            return {{"header", {{"type", "gameStarted"}}},
                    {"body", {{"text", "Game already started"}}}};
        }
        json errors;
        auto game = user.getGame();
        if (game == nullptr) {
            return errors;
        }
        if (game->getHostId() != user.getId()) {
            return {{"header", {{"type", "notHostError"}}},
                    {"body", {{"text", "User is not host of his game"}}}};
        }
        game->startGame();
        return {{"header", {{"type", "GameStartedSuccessfully"}}},
                {"body", {}}};
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

    loginQueriesMap["login"] = [&](const json &body) -> json {
        // basic checks
        std::cerr << "Entered\n";
        if (!body.contains("name") || !body["name"].is_string()) {
            return {{"header", {{"type", "wrongFormatError"}}},
                    {"body", {{"text", "can't find \"name\""}}}};
        }
        std::string name = body["name"];
        if (!body.contains("password") || !body["password"].is_string()) {
            return {{"header", {{"type", "wrongFormatError"}}},
                    {"body", {{"text", "can't find \"password\""}}}};
        }
        std::string password = body["password"];
        std::cerr << "Go to DB\n";
        auto result = userStorage->login(name, password);
        return result;
    };
    loginQueriesMap["register"] = [&](const json &body) -> json {
        // basic checks
        if (!body.contains("name") || !body["name"].is_string()) {
            return {{"header", {{"type", "wrongFormatError"}}},
                    {"body", {{"text", "can't find \"name\""}}}};
        }
        std::string name = body["name"];
        if (!body.contains("password") || !body["password"].is_string()) {
            return {{"header", {{"type", "wrongFormatError"}}},
                    {"body", {{"text", "can't find \"password\""}}}};
        }
        std::string password = body["password"];
        auto result = userStorage->registration(name, password);
        BOOST_LOG_TRIVIAL(debug) << result << '\n';
        return result;
    };
    loginQueriesMap["changePassword"] = [&](const json &body) -> json {
        // basic checks
        if (!body.contains("name") || !body["name"].is_string()) {
            return {{"header", {{"type", "wrongFormatError"}}},
                    {"body", {{"text", "can't find \"name\""}}}};
        }
        std::string name = body["name"];
        if (!body.contains("old_password") ||
            !body["old_password"].is_string()) {
            return {{"header", {{"type", "wrongFormatError"}}},
                    {"body", {{"text", "can't find \"old password\""}}}};
        }
        std::string old_password = body["old_password"];
        if (!body.contains("new_password") ||
            !body["new_password"].is_string()) {
            return {{"header", {{"type", "wrongFormatError"}}},
                    {"body", {{"text", "can't find \"new password\""}}}};
        }
        std::string new_password = body["new_password"];
        auto result =
            userStorage->changePassword(name, old_password, new_password);
        return result;
    };
}

void Server::polling() {
    std::cerr << "here\n";
    auto conn = std::make_shared<Connection>(ioContext, (*this));
    connections.push_back(conn);
    acceptor.async_accept(
        conn->socket(),
        std::bind(&Server::acceptHandler, this, conn, std::placeholders::_1));
}

void Server::acceptHandler(std::shared_ptr<Connection> conn,
                           boost::system::error_code e) {
    std::cerr << "enter\n";
    if (!e) {
        std::cout << "New client: " << conn->socket().remote_endpoint() << "->"
                  << conn->socket().local_endpoint() << std::endl;
        conn->readLine();
    }
    polling();
}

void Server::parseQuery(tcp::socket s) {
    /* try {
        Connection client(std::move(s), ioContext, (*this));
        std::cout << "New client: " << client.socket().remote_endpoint() <<
       "->"
                 << client.socket().local_endpoint() << std::endl;
       std::string line;
       json query, result;
       std::string user_name;
       std::optional<json> errors;
       try {
           while (client) {
               line = client.readLine();
               if (!client) {
                   std::cout
                       << "Disconnected: " << client.socket().remote_endpoint()
                       << "->" << client.socket().local_endpoint()
                       << std::endl;
                   return;
               }
               errors = checkQueryCorrectness(line);
               if (errors) {
                   client.writeLine(errors.value());
                   continue;
               }
               query = json::parse(line);
               auto queryHeader = query["header"];
               auto queryBody = query["body"];
               if (auto it = loginQueriesMap.find(queryHeader["type"]);
                   it != loginQueriesMap.end()) {
                   result = it->second(queryBody);
                   result["header"]["queryType"] = queryHeader["type"];
                   client.writeLine(result.dump() + '\n');
                   if ((result["header"]["queryType"] == "login" ||
                        result["header"]["queryType"] == "register") &&
                       result["header"]["type"] == "success") {
                       std::cerr << "successfully registered/logged in\n";
                       user_name = queryBody["name"].get<std::string>();
                       break;
                   }
               } else {
                   json header = json({{"type", "unknownQueryError"}});
                   json body = json();
                   result = json({{"header", header}, {"body", body}});
                   client.writeLine(result.dump() + '\n');
               }
           }
       } catch (nlohmann::detail::exception &e) {
           std::cerr << e.what() << std::endl;  // process error to client
       }

       User user(user_name, userStorage.get());

       try {
           while (client) {
               line = client.readLine();
               if (!client) {
                   break;
               }
               errors = checkQueryCorrectness(line);
               if (errors) {
                   client.writeLine(errors.value());
                   continue;
               }
               query = json::parse(line);
               auto queryHeader = query["header"];
               auto queryBody = query["body"];
               if (auto it = commonQueriesMap.find(queryHeader["type"]);
                   it != commonQueriesMap.end()) {
                   result = it->second(queryBody, user);
                   result["header"]["queryType"] = queryHeader["type"];
                   client.writeLine(result.dump() + '\n');
               } else {
                   json header = json({{"type", "unknownQueryError"}});
                   json body = json();
                   result = json({{"header", header}, {"body", body}});
                   client.writeLine(result.dump() + '\n');
               }
           }
       } catch (nlohmann::detail::exception &e) {
           std::cerr << e.what() << std::endl;  // process error to client
       }
       user.setGame(nullptr);
       std::cout << "Disconnected: " << client.socket().remote_endpoint()
                 << "->" << client.socket().local_endpoint() << std::endl;
}
catch (boost::system::system_error &e) {
    std::cerr << "Unknown Boost::ASIO error occurred: " << e.what() << '\n';
}
     */
}
std::optional<json> checkQueryCorrectness(const std::string &queryString) {
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

void Connection::readLine() {
    std::cerr << "entered readLine\n";
    boost::asio::streambuf buf;
    boost::asio::post(context, [&]() {
        boost::asio::async_read_until(
            mSocket, buf, '\n',
            [&](boost::system::error_code e, std::size_t btx) {
                std::cerr << "doRead?\n";
                if (e) {
                    std::cerr << e.message() << '\n';
                } else {
                    std::istream is(&readBuf);
                    std::string line;
                    std::getline(is, line);
                    std::cerr << line << '\n';

                    std::optional<json> errors = checkQueryCorrectness(line);
                    if (errors) {
                        writeLine(errors.value());
                        return;
                    }
                    readQueue.push_back(json::parse(line));
                    handleRead();
                    readLine();
                }
            });
    });
}

void Connection::doWrite(boost::system::error_code e) {
    if (e) {
        wasDisconnect = true;
    } else {
        writeQueue.pop_front();
        if (!writeQueue.empty()) {
            boost::asio::async_write(
                mSocket,
                boost::asio::buffer(writeQueue.front().data(),
                                    writeQueue.front().size()),
                std::bind(&Connection::doWrite, this, std::placeholders::_1));
        }
    }
}

void Connection::writeLine(std::string line) {
    boost::asio::post(context, [&]() {
        bool writeProgress = !writeQueue.empty();
        writeQueue.push_back(line);
        if (!writeProgress) {
            boost::asio::async_write(
                mSocket,
                boost::asio::buffer(writeQueue.front().data(),
                                    writeQueue.front().size()),
                std::bind(&Connection::doWrite, this, std::placeholders::_1));
        }
    });
}

void Connection::doRead(boost::system::error_code e, std::size_t btx) {
    std::cerr << "doRead?\n";
    if (e) {
        wasDisconnect = true;
    } else {
        std::istream is(&readBuf);
        std::string line;
        std::getline(is, line);

        std::optional<json> errors = checkQueryCorrectness(line);
        if (errors) {
            writeLine(errors.value());
            return;
        }
        readQueue.push_back(json::parse(line));
        handleRead();
        readLine();
    }
}

void Connection::handleRead() {
    while (!readQueue.empty()) {
        auto query = readQueue.front();
        readQueue.pop_front();
        if (!user) {
            handleLogin(query);
        } else {
            handleCommonQuery(query);
        }
    }
}

void Connection::handleLogin(const json &query) {
    auto queryHeader = query["header"];
    auto queryBody = query["body"];
    json result;
    if (auto it = server.loginQueriesMap.find(queryHeader["type"]);
        it != server.loginQueriesMap.end()) {
        result = it->second(queryBody);
        result["header"]["queryType"] = queryHeader["type"];
        writeLine(result.dump() + '\n');
        if ((result["header"]["queryType"] == "login" ||
             result["header"]["queryType"] == "register") &&
            result["header"]["type"] == "success") {
            std::cerr << "successfully registered/logged in\n";
            user.emplace(queryBody["name"].get<std::string>(),
                         server.userStorage.get());
        }
    } else {
        json header = json({{"type", "unknownQueryError"}});
        json body = json();
        result = json({{"header", header}, {"body", body}});
        writeLine(result.dump() + '\n');
    }
}

void Connection::handleCommonQuery(const json &query) {
    auto queryHeader = query["header"];
    auto queryBody = query["body"];
    json result;
    if (auto it = server.commonQueriesMap.find(queryHeader["type"]);
        it != server.commonQueriesMap.end()) {
        result = it->second(queryBody, user.value());
    } else {
        json header = json({{"type", "unknownQueryError"}});
        json body = json();
        result = json({{"header", header}, {"body", body}});
    }
    result["header"]["queryType"] = queryHeader["type"];
    writeLine(result.dump() + '\n');
}
}  // namespace FastTyping::Server