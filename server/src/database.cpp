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

    int Database::getId(const std::string &name)  { // creating new user or return existed one's id
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
        return find_by_name.size() != 0;
    }

    Database::~Database() {
        connect.disconnect();
    }
}