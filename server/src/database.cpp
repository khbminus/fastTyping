#include "database.h"
#include <algorithm>

namespace FastTyping::Server {
Database::Database() : connect("dbname = fast_typing") {
    try {
        if (connect.is_open()) {
            std::cerr << "Opened database successfully: " << connect.dbname()
                      << std::endl;
        } else {
            std::cerr << "Can't open database" << std::endl;
            std::abort();
        }

        /* Create SQL statement */
        sql =
            "CREATE TABLE IF NOT EXISTS USERS("
            "ID SERIAL PRIMARY KEY,"
            "NAME           TEXT    NOT NULL,"
            "PASSWORD           TEXT NOT NULL);";

        /* Create a transactional object. */

        pqxx::work W(connect);
        /* Execute SQL query */
        //            W.exec("DROP TABLE IF EXISTS mistakes;");
        W.exec(sql);
        sql =
            "CREATE TABLE IF NOT EXISTS MISTAKES("
            "ID SERIAL PRIMARY KEY,"
            "USER_ID INT    NOT NULL,"
            "KEYBOARD_TYPE TEXT NOT NULL,"
            "POST_DATE DATE NOT NULL DEFAULT CURRENT_DATE,"
            "MISTAKE  TEXT NOT NULL);";
        W.exec(sql);
        W.commit();
        std::cerr << "Table created successfully" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void Database::dropUsers() {
    std::unique_lock l{mutex};
    pqxx::work W(connect);
    W.exec("DROP TABLE USERS;");
    W.commit();
}

void Database::dropMistakes() {
    std::unique_lock l{mutex};
    pqxx::work W(connect);
    W.exec("DROP TABLE MISTAKES;");
    W.commit();
}
std::string Database::getPassword(int id) {
    std::unique_lock l{mutex};
    pqxx::work W(connect);
    sql = "SELECT PASSWORD FROM USERS WHERE ID = " + std::to_string(id) + ";";
    pqxx::result res{W.exec(sql)};
    std::string passw;
    for (auto row : res) {
        passw = row["PASSWORD"].c_str();
    }
    W.commit();
    return passw;
}

void Database::setPassword(int id, const std::string &passw) {
    std::unique_lock l{mutex};
    pqxx::work W(connect);
    W.exec("UPDATE USERS SET PASSWORD = '" + W.esc(passw) +
           "' WHERE ID = " + std::to_string(id) + ";");
    W.commit();
}

std::string Database::getName(int id) {
    std::unique_lock l{mutex};
    pqxx::work W(connect);
    sql = "SELECT NAME FROM USERS WHERE ID = " + std::to_string(id) + ";";
    pqxx::result res{W.exec(sql)};
    std::string name;
    for (auto row : res)
        name = row["NAME"].c_str();
    W.commit();
    return name;
}

int Database::getId(
    const std::string &name) {  // creating new user or return existed one's id
    std::unique_lock l{mutex};
    pqxx::work W(connect);
    pqxx::result find_by_name =
        W.exec("SELECT * FROM USERS WHERE NAME = '" + W.esc(name) + "';");

    if (find_by_name.empty()) {
        W.exec(
            "INSERT INTO USERS(NAME, PASSWORD) "
            "VALUES('" +
            W.esc(name) + "', '0000');");
        find_by_name =
            W.exec("SELECT * FROM USERS WHERE NAME = '" + W.esc(name) + "';");
    }
    int id = 0;

    for (auto row : find_by_name)
        id = row["ID"].as<int>();
    W.commit();
    return id;
}

bool Database::nameExist(const std::string &name) {
    std::unique_lock l{mutex};
    pqxx::work W(connect);
    pqxx::result find_by_name = W.exec(
        "SELECT * "
        "FROM USERS "
        "WHERE NAME = '" +
        W.esc(name) + "';");
    W.commit();
    return !find_by_name.empty();
}

Database::~Database() {
    connect.disconnect();
}

void Database::addMistake(int userId,
                          char let1,
                          char let2,
                          const std::string &keyboardType) {
    std::unique_lock l{mutex};
    pqxx::work W(connect);
    W.exec(
        "INSERT INTO MISTAKES(USER_ID, KEYBOARD_TYPE, MISTAKE)\n"
        "VALUES(" +
        std::to_string(userId) + ", '" + keyboardType + "', '" + let1 + let2 +
        "');");
    W.commit();
}

std::vector<std::pair<char, char>> Database::getTopMistakes(
    int userId,
    int number,
    const std::string &keyboardType) {
    std::unique_lock l{mutex};
    pqxx::work W(connect);
    std::vector<std::pair<char, char>> result;
    pqxx::result find_by_name = W.exec(
        "SELECT MISTAKE, COUNT(MISTAKE) AS value_occurrence "
        "FROM MISTAKES WHERE USER_ID = " +
        std::to_string(userId) + " AND KEYBOARD_TYPE = '" + keyboardType +
        "' GROUP BY MISTAKE "
        "ORDER BY value_occurrence DESC "
        "LIMIT " +
        std::to_string(number) + ";");
    std::for_each(find_by_name.begin(), find_by_name.end(),
                  [&](const pqxx::row &row) {
                      result.emplace_back(row["MISTAKE"].c_str()[0],
                                          row["MISTAKE"].c_str()[1]);
                  });
    W.commit();
    return result;
}

json Database::login(const std::string &name, const std::string &password) {
    int user_id;
    std::cerr << nameExist(name) << " ";
    std::cerr << getPassword(user_id = getId(name)) << " " << password << '\n';
    if (nameExist(name) && getPassword(user_id = getId(name)) == password) {
        return {{"header", {{"type", "success"}}}, {"body", {{"id", user_id}}}};
    }
    return {{"header", {{"type", "incorrectName"}}}, {"body", {{"id", -1}}}};
}

json Database::registration(const std::string &name,
                            const std::string &password) {
    if (!nameExist(name)) {
        int user_id = getId(name);
        setPassword(user_id, password);
        return {{"header", {{"type", "success"}}}, {"body", {{"id", user_id}}}};
    }
    return {{"header", {{"type", "nameAlreadyExists"}}},
            {"body", {{"id", -1}}}};
}

json Database::changePassword(const std::string &name,
                              const std::string &old_password,
                              const std::string &new_password) {
    int user_id;
    if (nameExist(name) && getPassword(user_id = getId(name)) == old_password) {
        setPassword(user_id, new_password);
        return {{"header", {{"type", "success"}}}, {"body", {{"id", user_id}}}};
    }
    return {{"header", {{"type", "incorrectName"}}}, {"body", {{"id", -1}}}};
}

}  // namespace FastTyping::Server