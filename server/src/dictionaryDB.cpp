#include "dictionaryDB.h"
#include <algorithm>
#include "constGame.h"

namespace FastTyping::Server {

void DictionariesDatabase::unanswered_query(std::string const &query) {
    pqxx::work work(connect);
    work.exec(query);
    work.commit();
}

void DictionariesDatabase::unanswered_query(
    std::vector<std::string> const &queries) {
    pqxx::work work(connect);
    for (auto const &query : queries) {
        work.exec(query);
    }
    work.commit();
}

const std::string create_table_const_query = R"sql(
CREATE TABLE IF NOT EXISTS CONST(
NAME    TEXT    NOT NULL,
VALUE   TEXT    NOT NULL);
)sql";

DictionariesDatabase::DictionariesDatabase() : connect("dbname = fast_typing") {
    try {
        if (connect.is_open()) {
            std::cerr << "Opened database successfully: " << connect.dbname()
                      << std::endl;
        } else {
            std::cerr << "Can't open database" << std::endl;
            std::abort();
        }
        unanswered_query(create_table_const_query);
        std::cerr << "Table created successfully" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void DictionariesDatabase::dropConst() {
    std::unique_lock l{mutex};
    unanswered_query("DROP TABLE CONST;");
}

std::string DictionariesDatabase::getLineConst(std::string const &name) {
    std::unique_lock l{mutex};
    pqxx::work work(connect);
    pqxx::result res = work.exec("SELECT * FROM CONST WHERE NAME = '" +
                                 connect.esc(name) + "';");
    work.commit();
    return (*res.begin())["VALUE"].c_str();
}

DictionariesDatabase::~DictionariesDatabase() {
    connect.disconnect();
}

void DictionariesDatabase::addConst(std::string const &name,
                                    std::string const &val) {
    std::unique_lock l{mutex};
    unanswered_query(
        "INSERT INTO CONST(NAME, VALUE)\n"
        "VALUES('" +
        connect.esc(name) + "', '" + connect.esc(val) + "');");
}
}  // namespace FastTyping::Server