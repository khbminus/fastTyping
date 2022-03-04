#include "doctest.h"
#include "server/include/user.h"
#include <json.hpp>
#include <type_traits>
using namespace FastTyping::Server;
TEST_CASE("User is constructible") {
    CHECK(std::is_constructible_v<User, json>);
    CHECK(std::is_constructible_v<User, std::string>);
}
TEST_CASE("User from json") {
    json j;
    j["username"] = "aboba";
    User u1(j);
    CHECK(u1.name() == "aboba");
    User u2(j);
    CHECK(u2.getId() == u1.getId() + 1);
    json failed;
    j["aboba"] = "obaba";
    json failed2;
    auto make_user = [](json js) { User kek = User(js); };
    CHECK_THROWS_AS(make_user(failed), nlohmann::detail::type_error);
    CHECK_THROWS_AS(make_user(failed2), nlohmann::detail::type_error);
    User u3(j);
    CHECK(u3.getId() == u2.getId() + 1);
}

TEST_CASE("user from string") {
    std::string name1 = "aboba";
    std::string name2 = "obaba";
    User u1(name1);
    User u2(name2);
    CHECK(u1.getId() + 1 == u2.getId());
    CHECK(u1.name() == name1);
    CHECK(u2.name() == name2);
}