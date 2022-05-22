#include "doctest.h"
#include "generator.h"


std::string path = "../../common/dictionary/english_top1000.txt";

generator::TextGenerator gen(path);

TEST_CASE("output") {
    std::vector<std::string> res = gen.getTop(5);
    CHECK(res.size() == 5);
    for (int i = 0; i < 5; i++) {
        std::cout << res[i] << '\n';
    }
}

TEST_CASE("Mistakes output") {
    std::vector<std::string> res = gen.getTop(5, true, {'a', 'c'});
    CHECK(res.size() == 5);
    for (int i = 0; i < 5; i++) {
        std::cout << res[i] << '\n';
    }
}
