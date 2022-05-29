#ifndef FASTTYPING_STATISTICSDB_H
#define FASTTYPING_STATISTICSDB_H

#include <mutex>
#include <nlohmann/json.hpp>
#include "database.h"
#include <pqxx/pqxx>
#include <vector>
#include "abc.h"

namespace FastTyping::Server {
struct StatisticsStorage {
    void dropStatistics();
    void addGame(int userId, const std::string dictName, double wpm, double rawWpm, int correctChars, int totalChars, double finishTime);
    StatisticsStorage();

private:
    Database &db;
};
}
#endif  // FASTTYPING_STATISTICSDB_H
