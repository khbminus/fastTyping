#ifndef FASTTYPING_DATABASE_H
#define FASTTYPING_DATABASE_H
#include <iostream>
#include <mutex>
#include <nlohmann/json.hpp>
#include <pqxx/pqxx>
#include <vector>

namespace FastTyping::Server {
using nlohmann::json;
struct Database {
public:
    Database();
    ~Database();
    // USERS table
    std::string getPassword(int id);
    std::string getName(int);
    void setPassword(int, const std::string &);  // base password == '0000'
    int getId(const std::string &);              // or create it
    bool nameExist(const std::string &);
    void dropUsers();
    // MISTAKES table
    void addMistake(int, char, char, const std::string &);
    std::vector<std::pair<char, char>> getTopMistakes(int userId,
                                                      int number,
                                                      const std::string &);
    void dropMistakes();

    // DICTIONARIES table
    void dropDictionaries();
    void addDictionary(std::string name, bool is_adaptable, std::string type);
    std::vector<std::string> get_dictionaries();

    // Queries
    json login(const std::string &name, const std::string &password);
    json registration(const std::string &name, const std::string &password);
    json changePassword(const std::string &name,
                        const std::string &old_password,
                        const std::string &new_password);

private:
    void unanswered_query(std::string const &query);
    void unanswered_query(std::vector<std::string> const &queries);
    mutable std::mutex mutex;
    pqxx::connection connect;
    std::string sql;  // sql query
};
}  // namespace FastTyping::Server

#endif  // FASTTYPING_DATABASE_H
