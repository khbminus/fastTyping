#ifndef FASTTYPING_USER_H
#define FASTTYPING_USER_H
#include "database.h"
#include "game_fwd.h"
#include <nlohmann/json.hpp>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>

using nlohmann::json;
namespace FastTyping::Server {
//    class DBUserStorage : public AbstractDatabase {
//    public:
//        DBUserStorage() : connect("dbname = fast_typing") {
//            try {
//                if (connect.is_open()) {
//                    std::cout << "Opened database successfully: " << connect.dbname() << std::endl;
//                } else {
//                    std::cout << "Can't open database" << std::endl;
//                }
//
//                /* Create SQL statement */
//                sql = "CREATE TABLE IF NOT EXISTS USERS("
//                      "ID INT PRIMARY KEY NOT NULL,"
//                      "NAME           TEXT    NOT NULL,"
//                      "PASSWORD           TEXT NOT NULL);";
//
//                /* Create a transactional object. */
//
//                pqxx::work W(connect);
//                /* Execute SQL query */
//                W.exec(sql);
//                W.commit();
//                std::cerr << "Table created successfully" << std::endl;
//            } catch (const std::exception &e) {
//                std::cerr << e.what() << std::endl;
//            }
//        }
//
//        std::string getPassword(int id) override {
//            std::unique_lock l{mutex};
//            pqxx::work W(connect);
//            sql = "SELECT PASSWORD FROM USERS WHERE ID = " + std::to_string(id) + ";";
//            pqxx::result res{W.exec(sql)};
//            std::string passw;
//            for (auto row: res) {
//                passw = row["PASSWORD"].c_str();
//            }
//            W.commit();
//            return passw;
//        }
//
//        void setPassword(int id, const std::string &passw) override {
//            std::unique_lock l{mutex};
//            pqxx::work W(connect);
//            W.exec("UPDATE USERS SET PASSWORD = '" + W.esc(passw) + "' WHERE ID = " + std::to_string(id) + ";");
//            W.commit();
//        }
//
//        std::string getName(int id) override {
//            std::unique_lock l{mutex};
//            pqxx::work W(connect);
//            sql = "SELECT NAME FROM USERS WHERE ID = " + std::to_string(id) + ";";
//            pqxx::result res{W.exec(sql)};
//            std::string name;
//            for (auto row: res)
//                name = row["NAME"].c_str();
//            W.commit();
//            return name;
//        }
//
//        int getId(const std::string &name) override {// creating new user or return existed one
//            std::unique_lock l{mutex};
//            pqxx::work W(connect);
//            int id;
//            pqxx::result find_by_name = W.exec("SELECT * FROM USERS WHERE NAME = '" + W.esc(name) + "';");
//
//            if (find_by_name.size() == 0) {
//                pqxx::result full_list = W.exec("SELECT * FROM USERS;");
//                id = full_list.size() + 1;
//                W.exec("INSERT INTO USERS(ID, NAME, PASSWORD)\n"
//                       "VALUES(" +
//                       std::to_string(id) + ", '" + W.esc(name) + "', '0000');");
//            } else {
//                for (auto row: find_by_name)
//                    id = row["ID"].as<int>();
//            }
//            W.commit();
//            return id;
//        }
//
//        bool nameExist(const std::string &name) override {
//            std::unique_lock l{mutex};
//            pqxx::work W(connect);
//            pqxx::result find_by_name = W.exec("SELECT * FROM USERS WHERE NAME = '" + W.esc(name) + "';");
//            return find_by_name.size() != 0;
//        }
//
//        ~DBUserStorage() override {
//            connect.disconnect();
//        }
//
//    private:
//        mutable std::mutex mutex;
//        pqxx::connection connect;
//        std::string sql;// sql query
//    };

    class DBUser {
    public:
        explicit DBUser(std::string &name_, Database *DB_) : userName(name_), DB(DB_), id(DB->getId(name_)), gameId(-1), wantToExit(false) {
        }

        [[nodiscard]] const std::string &name() const noexcept {
            return userName;
        }

        [[nodiscard]] int getId() const noexcept {
            return id;
        }

        [[nodiscard]] int isWantToExit() const noexcept {
            return (wantToExit ? 1 : 0);
        }
        void setGameId(int gameId_) noexcept {
            gameId = gameId_;
        }
        [[nodiscard]] int getGameId() const noexcept {
            return gameId;
        }
        //        [[nodiscard]] Game *getGame() const {
        //            return currentGame.get();
        //        }
        //        void setGame(std::shared_ptr<Game> game) {
        //            std::unique_lock l{mutex};
        //            currentGame = std::move(game);
        //        }
        void setWillToExit() noexcept {
            wantToExit = true;
        }
        void clearWillToExit() noexcept {
            wantToExit = false;
        }

    private:
        std::string userName;
        int gameId;
        bool wantToExit;
        Database *DB;
        int id = 0;
    };

    struct User : DBUser {
        User() = delete;
        explicit User(std::string &name_, Database *DB_) : DBUser(name_, DB_) {
        }
        [[nodiscard]] Game *getGame() const {
            // getGameId()
            return currentGame.get();
        }
        void setGame(std::shared_ptr<Game> game) {
            std::unique_lock l{mutex};
            currentGame = std::move(game);
            // setGameId(TODO)
        }

    private:
        std::shared_ptr<Game> currentGame = nullptr;
        mutable std::mutex mutex;
    };
}// namespace FastTyping::Server
#endif//FASTTYPING_USER_H
