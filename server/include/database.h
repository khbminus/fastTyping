#ifndef FASTTYPING_DATABASE_H
#define FASTTYPING_DATABASE_H
#include <iostream>
#include <mutex>
#include <pqxx/pqxx>

namespace FastTyping::Server {
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
    void addMistake(int, char, char, std::string);
    std::vector<std::pair<char, char>> getTopMistakes(int, int, std::string);
    void dropMistakes();

private:
    mutable std::mutex mutex;
    pqxx::connection connect;
    std::string sql;  // sql query
};
}  // namespace FastTyping::Server

#endif  // FASTTYPING_DATABASE_H
