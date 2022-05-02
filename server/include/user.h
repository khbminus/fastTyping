#ifndef FASTTYPING_USER_H
#define FASTTYPING_USER_H
#include "game_fwd.h"
#include <iostream>
#include <json.hpp>
#include <mutex>
#include <pqxx/pqxx>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>

using nlohmann::json;
using namespace pqxx;
namespace FastTyping::Server {
    class AbstractUserStorage {
    public:
        AbstractUserStorage() = default;
        virtual std::string getName(int) = 0;
        virtual std::string getPassword(int) = 0;
        virtual void setPassword(int, const std::string &) = 0;// base password == '0000'
        virtual int getId(const std::string &) = 0;            // or create it
        virtual void setWantExit(int) = 0;
        virtual bool nameExist(const std::string &) = 0;
        virtual void unsetWantExit(int) = 0;
        virtual bool getWantExit(int) = 0;
        virtual ~AbstractUserStorage() = default;
    };

    class DBUserStorage : public AbstractUserStorage {
    public:
        DBUserStorage() : C("dbname = fast_typing") {
            try {
                if (C.is_open()) {
                    std::cout << "Opened database successfully: " << C.dbname() << std::endl;
                } else {
                    std::cout << "Can't open database" << std::endl;
                }

                /* Create SQL statement */
                sql = "CREATE TABLE IF NOT EXISTS USERS("
                      "ID INT PRIMARY KEY NOT NULL,"
                      "NAME           TEXT    NOT NULL,"
                      "PASSWORD           TEXT    NOT NULL,"
                      "WANT_EXIT boolean);";

                /* Create a transactional object. */

                pqxx::work W(C);
                /* Execute SQL query */
                W.exec(sql);
                W.commit();
                std::cerr << "Table created successfully" << std::endl;
            } catch (const std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
        }

        std::string getPassword(int id) override {
            std::unique_lock l{mutex};
            pqxx::work W(C);
            sql = "SELECT PASSWORD FROM USERS WHERE ID = " + to_string(id) + ";";
            pqxx::result res{W.exec(sql)};
            std::string passw;
            for (auto row: res)
                passw = row["PASSWORD"].c_str();
            W.commit();
            return passw;
        }

        void setPassword(int id, const std::string &passw) override {
            std::unique_lock l{mutex};
            pqxx::work W(C);
            sql = "UPDATE USERS SET PASSWORD = '" + passw + "' WHERE ID = " + to_string(id) + ";";
            W.exec(sql);
            W.commit();
        }

        std::string getName(int id) override {
            std::unique_lock l{mutex};
            pqxx::work W(C);
            sql = "SELECT NAME FROM USERS WHERE ID = " + to_string(id) + ";";
            pqxx::result res{W.exec(sql)};
            std::string name;
            for (auto row: res)
                name = row["NAME"].c_str();
            W.commit();
            return name;
        }

        int getId(const std::string &name) override {// creating new user or return existed one
            std::unique_lock l{mutex};
            pqxx::work W(C);
            int id;
            pqxx::result find_by_name = W.exec("SELECT * FROM USERS WHERE NAME = '" + name + "';");

            if (find_by_name.size() == 0) {
                pqxx::result full_list = W.exec("SELECT * FROM USERS;");
                id = full_list.size() + 1;

                sql = "INSERT INTO USERS(ID, NAME, PASSWORD, WANT_EXIT)\n"
                      "SELECT " +
                      to_string(id) + ", '" + name + "', '0000', FALSE\n"
                                                     "WHERE\n"
                                                     "    NOT EXISTS (\n"
                                                     "        SELECT ID FROM USERS WHERE ID = " +
                      to_string(id) + ");";
                W.exec(sql);
            } else {
                for (auto row: find_by_name)
                    id = row["ID"].as<int>();
            }
            W.commit();
            return id;
        }

        bool nameExist(const std::string &name) override {
            std::unique_lock l{mutex};
            pqxx::work W(C);
            pqxx::result find_by_name = W.exec("SELECT * FROM USERS WHERE NAME = '" + name + "';");
            return find_by_name.size() != 0;
        }

        bool getWantExit(int id) override {
            std::unique_lock l{mutex};
            pqxx::work W(C);
            sql = "SELECT WANT_EXIT FROM USERS WHERE ID = " + to_string(id) + ";";
            pqxx::result res{W.exec(sql)};
            bool want_exit;
            for (auto row: res)
                want_exit = row["NAME"].as<bool>();
            W.commit();
            return want_exit;
        }

        void setWantExit(int id) override {
            std::unique_lock l{mutex};
            pqxx::work W(C);
            sql = "UPDATE USERS SET WANT_EXIT = TRUE WHERE ID = " + to_string(id) + ";";
            W.exec(sql);
            W.commit();
        }

        void unsetWantExit(int id) override {
            std::unique_lock l{mutex};
            pqxx::work W(C);
            sql = "UPDATE USERS SET WANT_EXIT = FALSE WHERE ID = " + to_string(id) + ";";
            W.exec(sql);
            W.commit();
        }

        ~DBUserStorage() override {
            C.disconnect();
        }

    private:
        mutable std::mutex mutex;
        pqxx::connection C;
        std::string sql;// sql query
    };

    class DBUser {
    public:
        explicit DBUser(std::string &name_, DBUserStorage *DB_) : userName(name_), DB(DB_), id(DB->getId(name_)), gameId(-1) {
        }

        [[nodiscard]] const std::string &name() const {
            return userName;
        }

        [[nodiscard]] int getId() const noexcept {
            return id;
        }

        [[nodiscard]] int isWantToExit() const noexcept {
            return DB->getWantExit(id);
        }
        void setGameId(int gameId_) {
            gameId = gameId_;
        }
        [[nodiscard]] int getGameId() const {
            return gameId;
        }
        //        [[nodiscard]] Game *getGame() const {
        //            return currentGame.get();
        //        }
        //        void setGame(std::shared_ptr<Game> game) {
        //            std::unique_lock l{mutex};
        //            currentGame = std::move(game);
        //        }
        void setWillToExit() {
            DB->setWantExit(id);
        }
        void clearWillToExit() {
            DB->unsetWantExit(id);
        }

    private:
        std::string userName;
        int gameId;
        DBUserStorage *DB;
        int id = 0;
    };

    struct User : DBUser {
        User() = delete;
        explicit User(std::string &name_, DBUserStorage *DB_) : DBUser(name_, DB_) {
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
