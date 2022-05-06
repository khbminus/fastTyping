#include "doctest.h"
#include "generator.h"
#include <type_traits>
using namespace generator;

std::string path = "../../common/dictionary/english_top1000.txt";

AdaptiveTextGenerator gen(path);

TEST_CASE("output") {
    std::vector<std::string> res = gen.getTop(5);
    assert(res.size() == 5);
    for (int i =0; i < 5; i++) {
        std::cout << res[i] << '\n';
    }
}

TEST_CASE("Mistakes output") {
    std::vector<std::string> res = gen.getTopByMistakes({'a', 'c'}, 5);
    assert(res.size() == 5);
    for (int i =0; i < 5; i++) {
        std::cout << res[i] << '\n';
    }
}
