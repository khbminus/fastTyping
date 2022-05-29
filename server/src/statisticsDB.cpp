#include "statisticsDB.h"
#include <algorithm>

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
FINISH_TIME NUMERIC(4, 2)
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

}  // namespace FastTyping::Server