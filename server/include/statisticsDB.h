#ifndef FASTTYPING_STATISTICSDB_H
#define FASTTYPING_STATISTICSDB_H

#include <mutex>
#include <nlohmann/json.hpp>
#include <pqxx/pqxx>
#include <vector>
#include "abc.h"
#include "database.h"

namespace FastTyping::Server {
struct StatisticsStorage {
    void dropStatistics();
    void addGame(int userId,
                 const std::string &dictName,
                 double wpm,
                 double rawWpm,
                 int correctChars,
                 int totalChars,
                 double finishTime);
    StatisticsStorage();

private:
    Database &db;
};
}  // namespace FastTyping::Server
#endif  // FASTTYPING_STATISTICSDB_H
