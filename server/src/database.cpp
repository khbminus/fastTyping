#include <iostream>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

int main(int argc, char *argv[]) {
    std::string sql;

    try {
        pqxx::connection C("dbname = fast_typing");
        if (C.is_open()) {
            cout << "Opened database successfully: " << C.dbname() << endl;
        } else {
            cout << "Can't open database" << endl;
            return 1;
        }

        /* Create SQL statement */
        sql = "CREATE TABLE IF NOT EXISTS USERS3("
              "ID INT PRIMARY KEY NOT NULL,"
              "NAME TEXT NOT NULL,"
              "WANT_EXIT boolean,"
              "GAME_ID INT);";

        /* Create a transactional object. */
        pqxx::work W{C};

        /* Execute SQL query */
        W.exec(sql);
        //        W.commit();
        W.commit();
        pqxx::work W2{C};

        cout << "Table created successfully" << endl;
        //        sql = "INSERT INTO USERS (ID, NAME, GAME_ID)\n"
        //              "VALUES(10,'zaya', 123);";
        //                W.exec(sql);
        //        W.commit();
        std::string name = "ZAJA6";
        pqxx::result r1 = W2.exec("SELECT * FROM USERS3 WHERE NAME = '" + name + "';");
        if (r1.size() == 0) {
            pqxx::result r2 = W2.exec("SELECT * FROM USERS3;");
            int const id = r2.size() + 1;
            sql = "INSERT INTO USERS3(ID, NAME, GAME_ID, WANT_EXIT)\n"
                  "SELECT " +
                  to_string(id) + ", '" + name + "', -1, FALSE\n"
                                                 "WHERE\n"
                                                 "    NOT EXISTS (\n"
                                                 "        SELECT ID FROM USERS3 WHERE ID = " +
                  to_string(id) + ");";
            pqxx::result res{W2.exec(sql)};
        }

        sql = "SELECT NAME FROM USERS3 WHERE ID = " + std::to_string(2);
        //        auto result = W.exec(sql);
        pqxx::result r{W2.exec(sql)};
        for (auto row: r)
            std::cout
                    // Address column by name.  Use c_str() to get C-style string.
                    << row["name"].c_str();
        // Address column by zero-based index.  Use as<int>() to parse as int.
        //                    << row[1].as<std::string>()
        //                    << "."
        //                    << std::endl;
        W2.commit();
        C.pqxx::connection_base::disconnect();
    } catch (const std::exception &e) {
        cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}