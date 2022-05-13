#include <nlohmann/json.hpp>
#include <type_traits>
#include "doctest.h"
#include "user.h"
using namespace FastTyping::Server;

TEST_CASE("user from string") {
    Database *db = new Database;
    std::string name1 = "aboba";
    std::string name2 = "obaba";
    User u1(name1, db);
    User u2(name2, db);
    CHECK(u1.getId() + 1 == u2.getId());
    CHECK(u1.name() == name1);
    CHECK(u2.name() == name2);
    delete db;
}

TEST_CASE("Database") {
    std::string name1 = "Aboba";
    std::string name2 = "Boba";
    Database storage;
    int a = storage.getId(name1);
    int b = storage.getId(name2);
    CHECK(a + 1 == b);
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
    Database storage;
    int a = storage.getId(name1);
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
    Database storage;
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