#ifndef DICTIONARY_DATABASE_H
#define DICTIONARY_DATABASE_H
#include <iostream>
#include <mutex>
#include <nlohmann/json.hpp>
#include <pqxx/pqxx>
#include <vector>
#include "abc.h"

namespace FastTyping::Server {
struct DictionariesDatabase {
public:
    DictionariesDatabase();
    ~DictionariesDatabase();
    void dropConst();
    void addConst(std::string const &name, std::string const &val);
    std::string getLineConst(std::string const &name);

private:
    void unanswered_query(std::string const &query);
    void unanswered_query(std::vector<std::string> const &queries);
    mutable std::mutex mutex;
    pqxx::connection connect;
};
}  // namespace FastTyping::Server

#endif  // FASTTYPING_DATABASE_H
