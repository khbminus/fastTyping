#include <boost/config.hpp>
#include "dictionaries_api.h"

struct const_dictionary : public dictionary_plugin {
    const std::vector<std::string> words() override {
        return {"Hello", "from", "dll"};
    }
};

extern "C" BOOST_SYMBOL_EXPORT const_dictionary dictionary;
const_dictionary dictionary;
