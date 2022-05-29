#include "dictionaryDB.h"
#include <algorithm>
#include "constGame.h"

namespace FastTyping::Server {
const std::string create_table_const_query = R"sql(
CREATE TABLE IF NOT EXISTS CONST_DICTIONARIES(
NAME    TEXT    NOT NULL,
VALUE   TEXT    NOT NULL);
)sql";

ConstDictionariesStorage::ConstDictionariesStorage()
    : db(Database::get_instance()) {
    try {
        db.unanswered_query(create_table_const_query);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void ConstDictionariesStorage::dropConst() {
    db.unanswered_query("DROP TABLE CONST_DICTIONARIES;");
}

std::string ConstDictionariesStorage::getLineConst(std::string const &name) {
    return db.get_column(
        "SELECT VALUE FROM CONST WHERE NAME = '" + db.esc(name) + "';",
        "VALUE");
}

void ConstDictionariesStorage::addConst(std::string const &name,
                                        std::string const &val) {
    db.unanswered_query(
        "INSERT INTO CONST(NAME, VALUE)\n"
        "VALUES('" +
        db.esc(name) + "', '" + db.esc(val) + "');");
}

std::unique_ptr<::FastTyping::Logic::AbstractDictionary> dictionary_instance(
    std::string const &name) {
    std::cout << "here!" << std::endl;
    DictionariesStorage dictionaries;

    if (!dictionaries.dictionaryExists(name)) {
        return nullptr;
    }

    std::string type = dictionaries.getType(name);

    if (type == "const") {
        ConstDictionariesStorage values;
        return std::make_unique<FastTyping::Logic::Dictionary>(
            values.getLineConst(name));
    }
    return std::make_unique<FastTyping::Logic::Dictionary>(
        "Placeholder, placeholder, placeholder!");
}
}  // namespace FastTyping::Server