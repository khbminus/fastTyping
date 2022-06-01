#ifndef DICTIONARIES_API_H
#define DICTIONARIES_API_H

#include <boost/config.hpp>
#include <string>
#include <vector>

struct BOOST_SYMBOL_VISIBLE dictionary_plugin {
    virtual const std::vector<std::string> words() = 0;
    virtual ~dictionary_plugin() = default;
};

#endif  // API_H_
