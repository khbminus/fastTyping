#ifndef FASTTYPING_DATABASE_H
#define FASTTYPING_DATABASE_H
#include <iostream>
#include <mutex>
#include <nlohmann/json.hpp>
#include <pqxx/pqxx>
#include <vector>
#include "abc.h"

namespace FastTyping::Server {

using nlohmann::json;

struct Database {
public:
    static Database &get_instance() {
        static Database instance;
        return instance;
    }
    ~Database();
    // DICTIONARIES table
    std::unique_ptr<::FastTyping::Logic::AbstractDictionary> get_dictionary(
        std::string const &name);
private:
    friend struct UserStorage;
    friend struct DictionariesStorage;
    friend struct ConstDictionariesStorage;
    friend struct MistakesStorage;
    Database();
    std::string esc(std::string const &raw);
    template <typename T>
    T get_column(std::string const &query, std::string const &column);
    std::string get_column(std::string const &query, std::string const &column);
    bool record_exists(std::string const &query);
    void unanswered_query(std::string const &query);
    void unanswered_query(std::vector<std::string> const &queries);
    mutable std::mutex mutex;
    pqxx::connection connect;
    // std::string sql;  // sql query
};

struct UserStorage {
    std::string getPassword(int id);
    std::string getName(int);
    void setPassword(int user_id,
                     const std::string &password);  // base password == '0000'
    int getId(const std::string &);              // or create it
    bool nameExist(const std::string &);
    void dropUsers();
    UserStorage();
private:
    Database &db;
};

struct DictionariesStorage {
    void dropDictionaries();
    void addDictionary(std::string name, bool is_adaptable, std::string type);
    std::vector<std::string> get_dictionaries();
    bool dictionaryExists(std::string const &dictionary_name);
    std::string getType(std::string const& name);
    DictionariesStorage();
private:
    Database &db;
};

struct MistakesStorage {
    void addMistake(int, char, char, const std::string &);
    std::vector<std::pair<char, char>> getTopMistakes(int userId,
                                                      int number,
                                                      const std::string &);
    void dropMistakes();
    MistakesStorage();
private:
    Database &db;
};

}  // namespace FastTyping::Server

#endif  // FASTTYPING_DATABASE_H
