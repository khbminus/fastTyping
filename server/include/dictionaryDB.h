#ifndef DICTIONARY_DATABASE_H
#define DICTIONARY_DATABASE_H
#include <iostream>
#include <mutex>
#include <nlohmann/json.hpp>
#include <pqxx/pqxx>
#include <vector>
#include <memory>
#include "abc.h"
#include "database.h"

namespace FastTyping::Server {
struct ConstDictionariesStorage {
public:
    ConstDictionariesStorage();
    void dropConst();
    void addConst(std::string const &name, std::string const &val);
    std::string getLineConst(std::string const &name);
private:
    Database& db;
};

std::unique_ptr<::FastTyping::Logic::AbstractDictionary> dictionary_instance(std::string const& name);

}  // namespace FastTyping::Server

#endif  // FASTTYPING_DATABASE_H
