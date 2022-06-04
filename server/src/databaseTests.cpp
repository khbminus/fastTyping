#include <iostream>
#include <nlohmann/json.hpp>
#include <type_traits>
#include "dictionaryDB.h"
#include "doctest.h"
#include "statisticsDB.h"
#include "user.h"

namespace FastTyping::Server {

bool operator==(GameStatistics lhs, GameStatistics rhs) {
    return std::tie(lhs.userId, lhs.dictName, lhs.wpm, lhs.rawWpm,
                    lhs.correctChars, lhs.totalChars, lhs.finishTime) ==
           std::tie(rhs.userId, rhs.dictName, rhs.wpm, rhs.rawWpm,
                    rhs.correctChars, rhs.totalChars, rhs.finishTime);
}
bool operator!=(const GameStatistics &lhs, const GameStatistics &rhs) {
    return !(rhs == lhs);
}

bool operator==(const DictStatistics &lhs, const DictStatistics &rhs) {
    return std::tie(lhs.userId, lhs.dictName, lhs.maxWpm, lhs.avgWpm,
                    lhs.avgAccuracy, lhs.sumFinishTime, lhs.gamesCnt) ==
           std::tie(rhs.userId, rhs.dictName, rhs.maxWpm, rhs.avgWpm,
                    rhs.avgAccuracy, rhs.sumFinishTime, rhs.gamesCnt);
}
bool operator!=(const DictStatistics &lhs, const DictStatistics &rhs) {
    return !(rhs == lhs);
}
// cppcheck-supress syntaxError
TEST_CASE("user from string") {
    // Database& db = Database::get_instance();
    UserStorage db;
    std::string name1 = "aboba";
    std::string name2 = "obaba";
    User u1(name1, db);
    User u2(name2, db);
    CHECK(u1.getId() + 1 == u2.getId());
    CHECK(u1.name() == name1);
    CHECK(u2.name() == name2);
}

TEST_CASE("Dictionaries") {
    DictionariesStorage flusher;
    flusher.dropDictionaries();

    DictionariesStorage storage;
    storage.addDictionary("Aboba", true, "const");
    storage.addDictionary("Boba", true, "const");
    auto dictionaries = storage.get_dictionaries();
    CHECK(storage.dictionaryExists("Aboba"));
    CHECK(storage.dictionaryExists("Boba"));
    CHECK(!storage.dictionaryExists("AbobaBoba"));
    CHECK(dictionaries ==
          std::vector{std::string("Aboba"), std::string("Boba")});
}

TEST_CASE("Database") {
    std::string name1 = "Aboba";
    std::string name2 = "Boba";
    UserStorage storage;
    int a = storage.getId(name1);
    int b = storage.getId(name2);
    CHECK(a + 1 == b);
    ConstDictionariesStorage d;
    //    CHECK(d.getLineConst("Aboba") == "This is sample don't judge me");
    CHECK(storage.getId(name1) == a);
    CHECK(storage.getId(name1) != b);
    CHECK(b == storage.getId(name2));
    CHECK(a != storage.getId(name2));
    CHECK(storage.getId(storage.getName(a)) == a);
    CHECK(storage.getId(storage.getName(a)) != b);
    CHECK(storage.getId(storage.getName(b)) == b);
    CHECK(storage.getId(storage.getName(b)) != a);
}

TEST_CASE("DB passwords") {
    std::string name1 = "Aboba";
    std::string name2 = "Boba";
    UserStorage storage;
    // Database& storage = Database::get_instance();
    int a = storage.getId(name1);
    CHECK(storage.nameExist(name1) == true);
    int b = storage.getId(name2);
    CHECK(storage.getPassword(a) == "0000");
    CHECK(storage.getPassword(b) == "0000");
    storage.setPassword(a, "lupa21_");
    CHECK(storage.getPassword(a) == "lupa21_");
    storage.setPassword(a, "aaaa");
    CHECK(storage.getPassword(a) == "aaaa");
    storage.setPassword(b, "0000");
    CHECK(storage.getPassword(b) == "0000");
    storage.setPassword(a, "0000");
    CHECK(storage.getPassword(a) == "0000");
}

TEST_CASE("DB mistakes") {
    MistakesStorage flusher;
    flusher.dropMistakes();
    MistakesStorage storage;

    storage.addMistake(1, 'e', 'e', "qwerty");
    storage.addMistake(0, 'c', 'b', "qwerty");
    storage.addMistake(0, 'a', 'b', "qwerty");
    storage.addMistake(0, 'a', 'a', "qwerty");
    storage.addMistake(0, 'a', 'b', "qwerty");
    storage.addMistake(0, 'a', 'a', "qwerty");
    storage.addMistake(0, 'a', 'a', "qwerty");
    storage.addMistake(0, 'c', 'c', "dvorak");

    std::vector<std::pair<char, char>> mistakes =
        storage.getTopMistakes(0, 10, "qwerty");
    CHECK(mistakes.size() == 3);
    CHECK(mistakes == std::vector<std::pair<char, char>>{
                          {'a', 'a'}, {'a', 'b'}, {'c', 'b'}});
    std::vector<std::pair<char, char>> mistakes_one =
        storage.getTopMistakes(0, 1, "qwerty");
    CHECK(mistakes_one.size() == 1);
    CHECK(mistakes_one == std::vector<std::pair<char, char>>{{'a', 'a'}});
}

TEST_CASE("Statistics") {
    StatisticsStorage flusher;
    flusher.dropStatistics();
    StatisticsStorage storage;
    storage.addGame(1, "const", 23.5, 25.55555, 10, 16, 30.2);
    storage.addGame(0, "const", 23.5, 25.55555, 10, 16, 30.2);
    storage.addGame(0, "const", 24.5, 25.55555, 10, 16, 28);
    storage.addGame(0, "const", 21, 25.55555, 10, 16, 30.3);
    CHECK(storage.getAvgWpm(0) == 23);
    CHECK(storage.getMaxWpm(0) == 24.5);
    CHECK(storage.getGamesAmount(0) == 3);

    CHECK(storage.getHistory(0) ==
          std::vector<GameStatistics>{
              {0, "const", 21, 25.56, 0.625, 10, 16, 30.3},
              {0, "const", 24.5, 25.56, 0.625, 10, 16, 28},
              {0, "const", 23.5, 25.56, 0.625, 10, 16, 30.2}});

    CHECK(storage.getHistory(0, 2) ==
          std::vector<GameStatistics>{
              {0, "const", 21, 25.56, 0.625, 10, 16, 30.3},
              {0, "const", 24.5, 25.56, 0.625, 10, 16, 28}});

    CHECK(storage.getUserDictStatistics(0) ==
          std::vector<DictStatistics>{{0, "const", 24.5, 23, 0.625, 88.5, 3}});

    CHECK(
        storage.getTopDictStatistics("const") ==
        std::vector<DictStatistics>{{0, "const", 24.5, 23, 0.625, 88.5, 3},
                                    {1, "const", 23.5, 23.5, 0.625, 30.2, 1}});

    CHECK(storage.getUserTotalStatistics(0) ==
          DictStatistics{0, "all", 24.5, 23, 0.625, 88.5, 3});
}
}  // namespace FastTyping::Server