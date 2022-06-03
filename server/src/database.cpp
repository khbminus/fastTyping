#include "database.h"
#include <algorithm>
#include <cstdlib>
#include <string>
#include "constGame.h"
#include "dictionaryDB.h"

// aboba
namespace FastTyping::Server {

std::string Database::esc(std::string const &raw) {
    return connect.esc(raw);
}

void Database::unanswered_query(std::string const &query) {
    std::unique_lock l{mutex};
    pqxx::work work(connect);
    work.exec(query);
    work.commit();
}

void Database::unanswered_query(std::vector<std::string> const &queries) {
    std::unique_lock l{mutex};
    pqxx::work work(connect);
    for (auto const &query : queries) {
        work.exec(query);
    }
    work.commit();
}

bool Database::record_exists(std::string const &query) {
    std::unique_lock l{mutex};
    pqxx::work work(connect);
    pqxx::result res = work.exec(query);
    work.commit();
    return !res.empty();
}

const std::string create_table_users_query = R"sql(
CREATE TABLE IF NOT EXISTS USERS(
ID SERIAL PRIMARY KEY,
NAME           TEXT    NOT NULL,
PASSWORD           TEXT NOT NULL);
)sql";

const std::string create_table_mistakes_query = R"sql(
CREATE TABLE IF NOT EXISTS MISTAKES(
ID SERIAL PRIMARY KEY,
USER_ID INT    NOT NULL,
KEYBOARD_TYPE TEXT NOT NULL,
POST_DATE DATE NOT NULL DEFAULT CURRENT_DATE,
MISTAKE  TEXT NOT NULL);
)sql";

const std::string create_table_dictionaries_query = R"sql(
CREATE TABLE IF NOT EXISTS DICTIONARIES (
ID SERIAL PRIMARY KEY,
NAME TEXT UNIQUE,
IS_ADAPTABLE BOOLEAN,
TYPE TEXT
);
)sql";

Database::Database()
    : connect("dbname = " + std::string(std::getenv("FASTTYPING_DB"))) {
    try {
        if (connect.is_open()) {
            std::cerr << "Opened database successfully: " << connect.dbname()
                      << std::endl;
        } else {
            std::cerr << "Can't open database" << std::endl;
            std::abort();
        }
        std::cerr << "Table created successfully" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

UserStorage::UserStorage() : db(Database::get_instance()) {
    try {
        db.unanswered_query(create_table_users_query);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void UserStorage::dropUsers() {
    db.unanswered_query("DROP TABLE USERS;");
}

bool UserStorage::nameExist(const std::string &name) {
    return db.record_exists(
        "SELECT 1 "
        "FROM USERS "
        "WHERE NAME = '" +
        db.esc(name) + "' LIMIT 1;");
}
void UserStorage::setPassword(int id, const std::string &passw) {
    db.unanswered_query("UPDATE USERS SET PASSWORD = '" + db.esc(passw) +
                        "' WHERE ID = " + std::to_string(id) + ";");
}

std::string UserStorage::getName(int id) {
    std::string sql =
        "SELECT NAME FROM USERS WHERE ID = " + std::to_string(id) + ";";
    return db.get_column<std::string>(sql, "NAME");
}

std::string UserStorage::getPassword(int id) {
    std::string sql =
        "SELECT PASSWORD FROM USERS WHERE ID = " + std::to_string(id) +
        " LIMIT 1;";
    return db.get_column<std::string>(sql, "PASSWORD");
}

// creating new user or return existed one's id
// TODO: refactor
int UserStorage::getId(const std::string &name) {
    std::unique_lock l{db.mutex};
    pqxx::work work(db.connect);
    pqxx::result find_by_name = work.exec("SELECT * FROM USERS WHERE NAME = '" +
                                          db.esc(name) + "' LIMIT 1;");

    if (find_by_name.empty()) {
        work.exec(
            "INSERT INTO USERS(NAME, PASSWORD) "
            "VALUES('" +
            work.esc(name) + "', '0000');");
        find_by_name = work.exec("SELECT * FROM USERS WHERE NAME = '" +
                                 db.esc(name) + "' LIMIT 1;");
    }
    int id = find_by_name.front()["ID"].as<int>();
    work.commit();
    return id;
}

DictionariesStorage::DictionariesStorage() : db(Database::get_instance()) {
    try {
        db.unanswered_query(create_table_dictionaries_query);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void DictionariesStorage::addDictionary(std::string const &name,
                                        bool is_adaptable,
                                        std::string const &type) {
    db.unanswered_query(
        "INSERT INTO DICTIONARIES(NAME, IS_ADAPTABLE, TYPE)\n"
        "VALUES('" +
        db.esc(name) + "', " + (is_adaptable ? "TRUE" : "FALSE") + ", '" +
        db.esc(type) + "');");
}

void DictionariesStorage::dropDictionaries() {
    db.unanswered_query("DROP TABLE DICTIONARIES;");
}

bool DictionariesStorage::dictionaryExists(std::string const &dictionary_name) {
    return db.record_exists(
        "SELECT 1 "
        "FROM DICTIONARIES "
        "WHERE NAME = '" +
        db.esc(dictionary_name) + "' LIMIT 1;");
}

std::string DictionariesStorage::getType(std::string const &name) {
    return db.get_column<std::string>(
        "SELECT TYPE "
        "FROM DICTIONARIES "
        "WHERE NAME = '" +
            db.esc(name) + "' LIMIT 1;",
        "TYPE");
}

std::vector<std::string> DictionariesStorage::get_dictionaries() {
    std::unique_lock l{db.mutex};
    pqxx::work work(db.connect);
    pqxx::result raw_result =
        work.exec("SELECT NAME FROM DICTIONARIES ORDER BY NAME;");

    std::vector<std::string> result;
    std::transform(raw_result.begin(), raw_result.end(),
                   std::back_inserter(result),
                   [](auto row) { return row["NAME"].c_str(); });
    work.commit();
    return result;
}

MistakesStorage::MistakesStorage() : db(Database::get_instance()) {
    try {
        std::cout << "creating table if t is not existing" << std::endl;
        db.unanswered_query(create_table_mistakes_query);
        std::cout << "table created" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void MistakesStorage::dropMistakes() {
    db.unanswered_query("DROP TABLE MISTAKES;");
}

void MistakesStorage::addMistake(int userId,
                                 char let1,
                                 char let2,
                                 const std::string &keyboardType) {
    db.unanswered_query(
        "INSERT INTO MISTAKES(USER_ID, KEYBOARD_TYPE, MISTAKE)\n"
        "VALUES(" +
        std::to_string(userId) + ", '" + db.esc(keyboardType) + "', '" + let1 +
        let2 + "');");
}

// TODO: refactor
std::vector<std::pair<char, char>> MistakesStorage::getTopMistakes(
    int userId,
    int number,
    const std::string &keyboardType) {
    std::unique_lock l{db.mutex};
    pqxx::work work(db.connect);
    std::vector<std::pair<char, char>> result;
    pqxx::result find_by_name = work.exec(
        "SELECT MISTAKE, COUNT(MISTAKE) AS value_occurrence "
        "FROM MISTAKES WHERE USER_ID = " +
        std::to_string(userId) + " AND KEYBOARD_TYPE = '" +
        db.esc(keyboardType) +
        "' GROUP BY MISTAKE "
        "ORDER BY value_occurrence DESC "
        "LIMIT " +
        std::to_string(number) + ";");
    std::transform(find_by_name.begin(), find_by_name.end(),
                   std::back_inserter(result), [](pqxx::row const &row) {
                       const char *data = row["MISTAKE"].c_str();
                       return std::make_pair(data[0], data[1]);
                   });
    work.commit();
    return result;
}

Database::~Database() {
    connect.disconnect();
}
}  // namespace FastTyping::Server