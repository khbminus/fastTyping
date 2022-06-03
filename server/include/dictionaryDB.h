#ifndef DICTIONARY_DATABASE_H
#define DICTIONARY_DATABASE_H
#include <iostream>
#include <memory>
#include "database.h"

namespace FastTyping::Server {
struct ConstDictionariesStorage {
public:
    ConstDictionariesStorage();

    void dropConst();

    void addConst(std::string const &name, std::string const &val);

    std::string getLineConst(std::string const &name);

private:
    Database &db;
};

struct FileDictionariesStorage {
public:
    FileDictionariesStorage();

    void dropFile();

    void addFile(std::string const &name, std::string const &filename);

    std::string getFileName(std::string const &name);

private:
    Database &db;
};

struct DLLDictionariesStorage {
public:
    DLLDictionariesStorage();

    void dropDLL();

    void addDLL(std::string const &name, std::string const &filename);

    std::string getDLLName(std::string const &name);

private:
    Database &db;
};

// corpus is a table in db;
struct CorpusDictionariesStorage {
public:
    CorpusDictionariesStorage();
    void dropCorpus();
    void addCorpus(std::string const &name, std::string const &filename);
    std::string getCorpusName(std::string const &name);

private:
    Database &db;
};

std::unique_ptr<::FastTyping::Logic::AbstractDictionary> dictionary_instance(
    std::string const &name,
    int user_id = -1,
    bool adapt = false);

}  // namespace FastTyping::Server

#endif  // FASTTYPING_DATABASE_H
