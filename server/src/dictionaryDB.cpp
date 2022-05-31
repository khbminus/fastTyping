#include "dictionaryDB.h"
#include <algorithm>
#include "constGame.h"
#include "dictionaries.h"

namespace FastTyping::Server {
const std::string create_table_const_query = R"sql(
CREATE TABLE IF NOT EXISTS CONST_DICTIONARIES (
NAME    TEXT    NOT NULL,
VALUE   TEXT    NOT NULL);
)sql";

const std::string create_table_file_query = R"sql(
CREATE TABLE IF NOT EXISTS FILE_DICTIONARIES (
NAME    TEXT    NOT NULL,
FILENAME   TEXT    NOT NULL);
)sql";

const std::string create_table_dll_query = R"sql(
CREATE TABLE IF NOT EXISTS DLL_DICTIONARIES (
NAME    TEXT    NOT NULL,
FILENAME   TEXT    NOT NULL);
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
    return db.get_column("SELECT VALUE FROM CONST_DICTIONARIES WHERE NAME = '" +
                             db.esc(name) + "';",
                         "VALUE");
}

void ConstDictionariesStorage::addConst(std::string const &name,
                                        std::string const &val) {
    db.unanswered_query(
        "INSERT INTO CONST_DICTIONARIES(NAME, VALUE)\n"
        "VALUES('" +
        db.esc(name) + "', '" + db.esc(val) + "');");
}

FileDictionariesStorage::FileDictionariesStorage()
    : db(Database::get_instance()) {
    try {
        db.unanswered_query(create_table_file_query);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void FileDictionariesStorage::dropFile() {
    db.unanswered_query("DROP TABLE FILE_DICTIONARIES;");
}

std::string FileDictionariesStorage::getFileName(std::string const &name) {
    return db.get_column(
        "SELECT FILENAME FROM FILE_DICTIONARIES WHERE NAME = '" + db.esc(name) +
            "';",
        "FILENAME");
}

void FileDictionariesStorage::addFile(std::string const &name,
                                      std::string const &filename) {
    db.unanswered_query(
        "INSERT INTO FILE_DICTIONARIES(NAME, FILENAME)\n"
        "VALUES('" +
        db.esc(name) + "', '" + db.esc(filename) + "');");
}

DLLDictionariesStorage::DLLDictionariesStorage()
    : db(Database::get_instance()) {
    try {
        db.unanswered_query(create_table_dll_query);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void DLLDictionariesStorage::dropDLL() {
    db.unanswered_query("DROP TABLE DLL_DICTIONARIES;");
}

std::string DLLDictionariesStorage::getDLLName(std::string const &name) {
    return db.get_column(
        "SELECT FILENAME FROM DLL_DICTIONARIES WHERE NAME = '" + db.esc(name) +
            "';",
        "FILENAME");
}

void DLLDictionariesStorage::addDLL(std::string const &name,
                                    std::string const &filename) {
    db.unanswered_query(
        "INSERT INTO DLL_DICTIONARIES(NAME, FILENAME)\n"
        "VALUES('" +
        db.esc(name) + "', '" + db.esc(filename) + "');");
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

    if (type == "file") {
        FileDictionariesStorage filenames;
        std::string filename = "dict/" + filenames.getFileName(name);
        return std::make_unique<FastTyping::Logic::FileDictionary>(filename);
    }

    if (type == "dll") {
        DLLDictionariesStorage dlls;
        std::string filename = dlls.getDLLName(name);
        return std::make_unique<FastTyping::Logic::DLLDictionary>(filename);
    }

    return std::make_unique<FastTyping::Logic::Dictionary>(
        "Placeholder, placeholder, placeholder!");
}
}  // namespace FastTyping::Server