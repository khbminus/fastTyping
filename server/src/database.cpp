#include "database.h"

namespace FastTyping::Server {
    Database::Database() : connect("dbname = fast_typing") {
        try {
            if (connect.is_open()) {
                std::cout << "Opened database successfully: " << connect.dbname() << std::endl;
            } else {
                std::cout << "Can't open database" << std::endl;
            }

            /* Create SQL statement */
            sql = "CREATE TABLE IF NOT EXISTS USERS("
                  "ID INT PRIMARY KEY NOT NULL,"
                  "NAME           TEXT    NOT NULL,"
                  "PASSWORD           TEXT NOT NULL);";

            /* Create a transactional object. */

            pqxx::work W(connect);
            /* Execute SQL query */
            //            W.exec("DROP TABLE IF EXISTS mistakes;");
            W.exec(sql);
            sql = "CREATE TABLE IF NOT EXISTS MISTAKES("
                  "ID SERIAL PRIMARY KEY,"
                  "USER_ID INT    NOT NULL,"
                  "POST_DATE DATE NOT NULL DEFAULT CURRENT_DATE,"
                  "MISTAKE  TEXT NOT NULL);";
            W.exec(sql);
            W.commit();
            std::cerr << "Table created successfully" << std::endl;
        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }

    std::string Database::getPassword(int id) {
        std::unique_lock l{mutex};
        pqxx::work W(connect);
        sql = "SELECT PASSWORD FROM USERS WHERE ID = " + std::to_string(id) + ";";
        pqxx::result res{W.exec(sql)};
        std::string passw;
        for (auto row: res) {
            passw = row["PASSWORD"].c_str();
        }
        W.commit();
        return passw;
    }

    void Database::setPassword(int id, const std::string &passw) {
        std::unique_lock l{mutex};
        pqxx::work W(connect);
        W.exec("UPDATE USERS SET PASSWORD = '" + W.esc(passw) + "' WHERE ID = " + std::to_string(id) + ";");
        W.commit();
    }

    std::string Database::getName(int id) {
        std::unique_lock l{mutex};
        pqxx::work W(connect);
        sql = "SELECT NAME FROM USERS WHERE ID = " + std::to_string(id) + ";";
        pqxx::result res{W.exec(sql)};
        std::string name;
        for (auto row: res)
            name = row["NAME"].c_str();
        W.commit();
        return name;
    }

    int Database::getId(const std::string &name) {// creating new user or return existed one's id
        std::unique_lock l{mutex};
        pqxx::work W(connect);
        int id;
        pqxx::result find_by_name = W.exec("SELECT * FROM USERS WHERE NAME = '" + W.esc(name) + "';");

        if (find_by_name.size() == 0) {
            pqxx::result full_list = W.exec("SELECT * FROM USERS;");
            id = full_list.size() + 1;
            W.exec("INSERT INTO USERS(ID, NAME, PASSWORD)\n"
                   "VALUES(" +
                   std::to_string(id) + ", '" + W.esc(name) + "', '0000');");
        } else {
            for (auto row: find_by_name)
                id = row["ID"].as<int>();
        }
        W.commit();
        return id;
    }

    bool Database::nameExist(const std::string &name) {
        std::unique_lock l{mutex};
        pqxx::work W(connect);
        pqxx::result find_by_name = W.exec("SELECT * FROM USERS WHERE NAME = '" + W.esc(name) + "';");
        W.commit();
        return find_by_name.size() != 0;
    }

    Database::~Database() {
        connect.disconnect();
    }

    void Database::addMistake(int userId, char let1, char let2) {
        std::unique_lock l{mutex};
        pqxx::work W(connect);
        W.exec("INSERT INTO MISTAKES(USER_ID, MISTAKE)\n"
               "VALUES(" +
               std::to_string(userId) + ", '" + let1 + let2 + "');");
        W.commit();
    }

    std::vector<std::pair<char, char>> Database::getTopMistakes(int userId, int number) {
        std::unique_lock l{mutex};
        pqxx::work W(connect);
        std::vector<std::pair<char, char>> result;
        pqxx::result find_by_name = W.exec("SELECT MISTAKE, COUNT(MISTAKE) AS value_occurrence "
                                           "FROM MISTAKES WHERE USER_ID = " +
                                           std::to_string(userId) + " GROUP BY MISTAKE "
                                                                    "ORDER BY value_occurrence DESC "
                                                                    "LIMIT " +
                                           std::to_string(number) + ";");
        for (auto row: find_by_name) {
            result.push_back({row["MISTAKE"].c_str()[0], row["MISTAKE"].c_str()[1]});
        }
        W.commit();
        return result;
    }


}// namespace FastTyping::Server