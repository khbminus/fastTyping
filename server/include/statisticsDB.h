#ifndef FASTTYPING_STATISTICSDB_H
#define FASTTYPING_STATISTICSDB_H

#include "database.h"

namespace FastTyping::Server {

struct gameStatistics {
    bool operator==(const gameStatistics &rhs) const;
    bool operator!=(const gameStatistics &rhs) const;
    int userId;
    std::string dictName;
    double wpm;
    double rawWpm;
    double accuracy;
    int correctChars;
    int totalChars;
    double finishTime;
};

struct dictStatistics {
    bool operator==(const dictStatistics &rhs) const;
    bool operator!=(const dictStatistics &rhs) const;
    int userId;
    const std::string dictName;
    double maxWpm;
    double avgWpm;
    double avgAccuracy;
    double sumFinishTime;
    int gamesCnt;
};

struct StatisticsStorage {
    void dropStatistics();
    void addGame(int userId,
                 const std::string &dictName,
                 double wpm,
                 double rawWpm,
                 int correctChars,
                 int totalChars,
                 double finishTime);

    double getMaxWpm(int userId);
    double getAvgWpm(int userId);
    int getGamesAmount(int userId);
    std::vector<gameStatistics> getHistory(int userI, int amount = 10);
    std::vector<dictStatistics> getUserDictStatistics(int userId);
    dictStatistics getUserTotalStatistics(int userId);
    std::vector<dictStatistics> getTopDictStatistics(
        const std::string &dictName);

    StatisticsStorage();

private:
    Database &db;
};
}  // namespace FastTyping::Server
#endif  // FASTTYPING_STATISTICSDB_H
