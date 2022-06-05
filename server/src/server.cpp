#include "server.h"
#include <boost/locale.hpp>
#include <boost/log/trivial.hpp>
#include <cctype>
#include <iostream>
#include <memory>
#include <optional>
#include <thread>
#include "dictionaryDB.h"
#include "game.h"

namespace FastTyping::Server {
Server::Server()
    : acceptor(ioContext, tcp::endpoint(tcp::v4(), PORT)),
      user_storage(new UserStorage),
      dictionaries_storage(new DictionariesStorage),
      mistakes_storage(new MistakesStorage),
      gameStorage(new MapGameStorage),
      statisticsStorage(new StatisticsStorage) {
    boost::locale::generator gen;
    std::locale::global(gen(""));

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

        bool adapt = false;

        if (body.contains("adapt") && body["adapt"].is_boolean() &&
            body["adapt"]) {
            adapt = true;
        }

        if (user.getGame() != nullptr) {
            return {{"header", {{"type", "alreadyInGameError"}}},
                    {"body", {{"text", "Already in game"}}}};
        }

        std::cout << "adapt = " << adapt << std::endl;

        auto result = gameStorage->createGame(body, user.getId(), adapt);
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
                game->joinUser(user.getId());
                result["body"]["joined"] = true;
                result["body"]["error"] = "";
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
        if (game->getGameStarted()) {
            return {{"header", {{"type", "connectAfterStartError"}}},
                    {"body", {{"text", "Try connect after game started"}}}};
        }
        if (game->hasUser(user.getId())) {
            return {{"header", {{"type", "connectAfterLeaveError"}}},
                    {"body", {{"text", "Try connect after disconnect"}}}};
        }
        user.setGame(game);
        game->joinUser(user.getId());
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

    commonQueriesMap["addTypos"] = [&](const json &body, User &user) -> json {
        if (!body.contains("typos")) {
            return {{"header", {{"type", "typosAdded"}}},
                    {"body", {{"text", "no typos"}}}};
        }
        auto typos = body["typos"].get<std::vector<std::string>>();

        int id = user.getId();
        for (auto const &typo : typos) {
            std::cout << "typo = '" << typo << "' " << typo.size() << std::endl;
            if (typo.size() == 2 && std::isalpha(typo[0]) &&
                std::isalpha(typo[1])) {
                std::cout << "add" << std::endl;
                mistakes_storage->addMistake(id, typo[0], typo[1], "qwerty");
            }
        }
        return {{"header", {{"type", "typosAdded"}}},
                {"body", {{"text", "no typos"}}}};
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
        auto *game = user.getGame();
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
        if (!body.contains("char") || !body["char"].is_string()) {
            return {{"header", {{"type", "wrongFormatError"}}},
                    {"body", {{"text", "can't find word to check"}}}};
        }

        auto normalized = boost::locale::normalize(
            body["char"].get<std::string>(), boost::locale::norm_nfkc);

        boost::locale::boundary::ssegment_index index(
            boost::locale::boundary::character, normalized.begin(),
            normalized.end());
        if (std::distance(index.begin(), index.end()) != 1) {
            return {{"header", {{"type", "notSingleChar"}}},
                    {"body", {{"text", "can't find word to check"}}}};
        }

        auto result = user.getGame()->addNewChar(user.getId(), normalized);
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

    commonQueriesMap["getUserName"] = [&](const json &body,
                                          User &user) -> json {
        if (!body.contains("userId") || !body["userId"].is_number_unsigned()) {
            return {{"header", {{"type", "userName"}}},
                    {"body", {{"userName", user.name()}}}};
        }
        if (!user_storage->idExist(body["userId"])) {
            return {{"header", {{"type", "userNotFound"}}}, {"body", {}}};
        }
        return {
            {"header", {{"type", "userName"}}},
            {"body", {{"userName", user_storage->getName(body["userId"])}}}};
    };

    commonQueriesMap["getGameStatistics"] = [&](const json &body,
                                                User &user) -> json {
        if (user.getGame() == nullptr) {
            return {{"header", {{"type", "notInGameError"}}},
                    {"body", {{"text", "not in game"}}}};
        }
        return user.getGame()->getStatistics(user.getId());
    };

    commonQueriesMap["getDictionaries"] = [&](json const &body,
                                              User &user) -> json {
        json result = json::object();
        result["header"] = {{"type", "dictionaries"},
                            {"queryType", "getDictionaries"}};
        result["body"] = {{"list", dictionaries_storage->get_dictionaries()}};
        return result;
    };

    commonQueriesMap["getProfile"] = [&](const json &body, User &user) -> json {
        json result = json::object();
        result["header"] = {{"type", "profileResult"}};
        auto res = statisticsStorage->getUserTotalStatistics(user.getId());
        result["body"] = {
            {"dictName", res.dictName},
            {"maxWpm", res.maxWpm},
            {"avgWpm", res.avgWpm},
            {"avgAccuracy", res.avgAccuracy},
            {"sumFinishTime", static_cast<int>(res.sumFinishTime)},
            {"gamesCnt", res.gamesCnt}};
        return result;
    };

    commonQueriesMap["getUserDictionaries"] = [&](const json &body,
                                                  User &user) -> json {
        json result = json::object();
        result["header"] = {{"type", "userDictionaries"}};
        auto res = statisticsStorage->getUserDictStatistics(user.getId());
        std::vector<json> resJson(res.size());
        std::transform(res.begin(), res.end(), resJson.begin(),
                       [&](const DictStatistics &stats) -> json {
                           return {{"dictName", stats.dictName},
                                   {"maxWpm", stats.maxWpm},
                                   {"avgWpm", stats.avgWpm},
                                   {"avgAccuracy", stats.avgAccuracy},
                                   {"sumFinishTime",
                                    static_cast<int>(stats.sumFinishTime)},
                                   {"gamesCnt", stats.gamesCnt}};
                       });
        result["body"] = resJson;
        return result;
    };
    commonQueriesMap["getUserGames"] = [&](const json &body,
                                           User &user) -> json {
        int limit = 10;
        if (body.contains("limit") && body["limit"].is_number_unsigned()) {
            limit = body["limit"];
        }

        json result = json::object();
        result["header"] = {{"type", "userGames"}};
        auto res = statisticsStorage->getHistory(user.getId(), limit);
        std::vector<json> resJson(res.size());
        std::transform(
            res.begin(), res.end(), resJson.begin(),
            [&](const GameStatistics &stats) -> json {
                return {{"dictName", stats.dictName},
                        {"wpm", stats.wpm},
                        {"rawWpm", stats.rawWpm},
                        {"accuracy", stats.accuracy},
                        {"finishTime", static_cast<int>(stats.finishTime)}};
            });
        result["body"] = resJson;
        return result;
    };

    commonQueriesMap["getDictionaryStats"] = [&](const json &body,
                                                 User &user) -> json {
        if (!body.contains("dictionaryName") ||
            !body["dictionaryName"].is_string()) {
            return {{"header", {{"type", "wrongFormatError"}}},
                    {"body", {{"text", "can't find dictionary name"}}}};
        }
        json result = json::object();
        result["header"] = {{"type", "dictionaryStats"}};
        auto res =
            statisticsStorage->getTopDictStatistics(body["dictionaryName"]);
        std::vector<json> resJson(res.size());
        std::transform(res.begin(), res.end(), resJson.begin(),
                       [&](const DictStatistics &stats) -> json {
                           return {{"dictName", stats.dictName},
                                   {"maxWpm", stats.maxWpm},
                                   {"avgWpm", stats.avgWpm},
                                   {"avgAccuracy", stats.avgAccuracy},
                                   {"sumFinishTime",
                                    static_cast<int>(stats.sumFinishTime)},
                                   {"gamesCnt", stats.gamesCnt},
                                   {"userId", stats.userId}};
                       });
        result["body"] = resJson;
        return result;
    };

    loginQueriesMap["login"] = [&](const json &body) -> json {
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
        int user_id{};
        if (user_storage->nameExist(name) &&
            user_storage->getPassword(user_id = user_storage->getId(name)) ==
                password) {
            return {{"header", {{"type", "success"}}},
                    {"body", {{"id", user_id}}}};
        }
        return {{"header", {{"type", "incorrectName"}}},
                {"body", {{"id", -1}}}};
    };

    loginQueriesMap["register"] = [&](const json &body) -> json {
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
        json result;

        if (!user_storage->nameExist(name)) {
            int user_id = user_storage->getId(name);
            user_storage->setPassword(user_id, password);
            result = {{"header", {{"type", "success"}}},
                      {"body", {{"id", user_id}}}};
        } else {
            result = {{"header", {{"type", "nameAlreadyExists"}}},
                      {"body", {{"id", -1}}}};
        }
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
        int user_id{};
        if (user_storage->nameExist(name) &&
            user_storage->getPassword(user_id = user_storage->getId(name)) ==
                old_password) {
            user_storage->setPassword(user_id, new_password);
            return {{"header", {{"type", "success"}}},
                    {"body", {{"id", user_id}}}};
        }
        return {{"header", {{"type", "incorrectName"}}},
                {"body", {{"id", -1}}}};
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
        json query, result;
        std::string user_name;
        std::optional<json> errors;
        try {
            while (client) {
                if (!std::getline(client, line)) {
                    std::cout
                        << "Disconnected: " << client.socket().remote_endpoint()
                        << "->" << client.socket().local_endpoint()
                        << std::endl;
                    return;
                }
                errors = checkQueryCorrectness(line);
                if (errors) {
                    client << errors.value() << '\n';
                    continue;
                }
                query = json::parse(line);
                auto queryHeader = query["header"];
                auto queryBody = query["body"];
                // BOOST_LOG_TRIVIAL(debug) << query << '\n';
                if (auto it = loginQueriesMap.find(queryHeader["type"]);
                    it != loginQueriesMap.end()) {
                    result = it->second(queryBody);
                    result["header"]["queryType"] = queryHeader["type"];
                    // BOOST_LOG_TRIVIAL(debug) << result << '\n';
                    client << result << '\n';
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
                    client << json({{"header", header}, {"body", body}})
                           << '\n';
                }
            }
        } catch (nlohmann::detail::exception &e) {
            std::cerr << e.what() << std::endl;  // process error to client
        }

        User user(user_name, *user_storage);

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
                // BOOST_LOG_TRIVIAL(debug) << query << '\n';
                if (auto it = commonQueriesMap.find(queryHeader["type"]);
                    it != commonQueriesMap.end()) {
                    result = it->second(queryBody, user);
                    result["header"]["queryType"] = queryHeader["type"];
                    client << result << '\n';
                } else {
                    json header = json({{"type", "unknownQueryError"}});
                    json body = json();
                    client << json({{"header", header}, {"body", body}})
                           << '\n';
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