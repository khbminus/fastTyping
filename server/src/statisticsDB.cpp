#include "statisticsDB.h"
#include <algorithm>
#include <iomanip>

namespace FastTyping::Server {

const std::string create_table_statistics_query = R"sql(
CREATE TABLE IF NOT EXISTS STATISTICS (
ID SERIAL PRIMARY KEY,
USER_ID INT,
DICT_NAME TEXT,
WPM NUMERIC(4, 2),
RAW_WPM NUMERIC(4, 2),
CORRECT_CHARS INT,
TOTAL_CHARS INT,
FINISH_TIME NUMERIC(4, 2),
POST_DATE TIMESTAMP NOT NULL DEFAULT CURRENT_DATE
);
)sql";

std::string to_numeric(double value) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << value;
    return stream.str();
}
StatisticsStorage::StatisticsStorage() : db(Database::get_instance()) {
    try {
        db.unanswered_query(create_table_statistics_query);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void StatisticsStorage::dropStatistics() {
    db.unanswered_query("DROP TABLE STATISTICS;");
}

void StatisticsStorage::addGame(int userId,
                                const std::string &dictName,
                                double wpm,
                                double rawWpm,
                                int correctChars,
                                int totalChars,
                                double finishTime) {
    db.unanswered_query(
        "INSERT INTO STATISTICS(USER_ID, DICT_NAME, WPM, RAW_WPM, "
        "CORRECT_CHARS, TOTAL_CHARS, FINISH_TIME)\n"
        "VALUES(" +
        std::to_string(userId) + ", '" + db.esc(dictName) + "', " +
        to_numeric(wpm) + ", " + to_numeric(rawWpm) + ", " +
        std::to_string(correctChars) + ", " + std::to_string(totalChars) +
        ", " + to_numeric(finishTime) + ");");
}

double StatisticsStorage::getMaxWpm(int userId) {
    return db.get_column<double>(
        "SELECT MAX(WPM) AS RESULT FROM STATISTICS WHERE USER_ID = " +
            std::to_string(userId) + ";",
        "RESULT");
}
double StatisticsStorage::getAvgWpm(int userId) {
    return db.get_column<double>(
        "SELECT Avg(WPM) AS RESULT FROM STATISTICS WHERE USER_ID = " +
            std::to_string(userId) + ";",
        "RESULT");
}
int StatisticsStorage::getGamesAmount(int userId) {
    return db.get_column<int>(
        "SELECT COUNT(*) AS RESULT FROM STATISTICS WHERE USER_ID = " +
            std::to_string(userId) + ";",
        "RESULT");
}

std::vector<GameStatistics> StatisticsStorage::getHistory(int userId,
                                                          int amount) {
    std::vector<GameStatistics> result;
    std::unique_lock l{db.mutex};
    pqxx::work work(db.connect);
    pqxx::result history = work.exec(
        "SELECT * FROM STATISTICS WHERE USER_ID = " + std::to_string(userId) +
        " ORDER BY ID DESC LIMIT " + std::to_string(amount) + ";");
    std::transform(history.begin(), history.end(), std::back_inserter(result),
                   [](pqxx::row const &row) {
                       return GameStatistics{
                           row["USER_ID"].as<int>(),
                           row["DICT_NAME"].as<std::string>(),
                           row["WPM"].as<double>(),
                           row["RAW_WPM"].as<double>(),
                           row["CORRECT_CHARS"].as<double>() /
                               row["TOTAL_CHARS"].as<double>(),
                           row["CORRECT_CHARS"].as<int>(),
                           row["TOTAL_CHARS"].as<int>(),
                           row["FINISH_TIME"].as<double>()};
                   });
    work.commit();
    return result;
}

std::vector<DictStatistics> StatisticsStorage::getUserDictStatistics(
    int userId) {
    std::vector<DictStatistics> result;
    std::unique_lock l{db.mutex};
    pqxx::work work(db.connect);
    pqxx::result raw_result = work.exec(
        "SELECT DICT_NAME, MAX(WPM) AS MAX_WPM, AVG(WPM) AS AVG_WPM, "
        "AVG(CORRECT_CHARS::NUMERIC / TOTAL_CHARS) AS AVG_ACCURACY, "
        "SUM(FINISH_TIME) AS "
        "SUM_TIME, COUNT(*) "
        "AS GAMES_CNT FROM STATISTICS WHERE USER_ID = " +
        std::to_string(userId) + " GROUP BY DICT_NAME;");

    std::transform(raw_result.begin(), raw_result.end(),
                   std::back_inserter(result), [userId](pqxx::row const &row) {
                       return DictStatistics{userId,
                                             row["DICT_NAME"].as<std::string>(),
                                             row["MAX_WPM"].as<double>(),
                                             row["AVG_WPM"].as<double>(),
                                             row["AVG_ACCURACY"].as<double>(),
                                             row["SUM_TIME"].as<double>(),
                                             row["GAMES_CNT"].as<int>()};
                   });
    work.commit();
    return result;
}

DictStatistics StatisticsStorage::getUserTotalStatistics(int userId) {
    std::unique_lock l{db.mutex};
    pqxx::work work(db.connect);
    pqxx::result raw_result = work.exec(
        "SELECT MAX(WPM) AS MAX_WPM, AVG(WPM) AS AVG_WPM, "
        "AVG(CORRECT_CHARS::NUMERIC / "
        "TOTAL_CHARS) AS AVG_ACCURACY, SUM(FINISH_TIME) AS SUM_TIME, COUNT(*) "
        "AS GAMES_CNT FROM STATISTICS WHERE USER_ID = " +
        std::to_string(userId) + ";");
    if (raw_result.size() == 0) {
        work.commit();
        return {userId, "all", 0, 0, 0, 0, 0};
    }
    DictStatistics result{userId,
                          "all",
                          raw_result[0]["MAX_WPM"].as<double>(),
                          raw_result[0]["AVG_WPM"].as<double>(),
                          raw_result[0]["AVG_ACCURACY"].as<double>(),
                          raw_result[0]["SUM_TIME"].as<double>(),
                          raw_result[0]["GAMES_CNT"].as<int>()};
    work.commit();
    return result;
}

std::vector<DictStatistics> StatisticsStorage::getTopDictStatistics(
    const std::string &dictName) {
    std::vector<DictStatistics> result;
    std::unique_lock l{db.mutex};
    pqxx::work work(db.connect);
    pqxx::result raw_result = work.exec(
        "SELECT USER_ID, MAX(WPM) AS MAX_WPM, AVG(WPM) AS AVG_WPM, "
        "AVG(CORRECT_CHARS::NUMERIC / TOTAL_CHARS) AS AVG_ACCURACY, "
        "SUM(FINISH_TIME) AS "
        "SUM_TIME, COUNT(*) AS "
        "GAMES_CNT FROM STATISTICS WHERE DICT_NAME = '" +
        db.esc(dictName) + "' GROUP BY USER_ID;");
    std::transform(
        raw_result.begin(), raw_result.end(), std::back_inserter(result),
        [dictName](pqxx::row const &row) {
            return DictStatistics{
                row["USER_ID"].as<int>(),         dictName,
                row["MAX_WPM"].as<double>(),      row["AVG_WPM"].as<double>(),
                row["AVG_ACCURACY"].as<double>(), row["SUM_TIME"].as<double>(),
                row["GAMES_CNT"].as<int>()};
        });
    work.commit();
    return result;
}

}  // namespace FastTyping::Server