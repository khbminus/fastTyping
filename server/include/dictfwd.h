#ifndef DICTIONARIES_FWD_H
#define DICTIONARIES_FWD_H

#include <fstream>
#include <string>
#include <vector>

namespace FastTyping::Logic {

struct CorpusDictionary;
void add_corpus_dictionary(std::string const &name,
                           std::vector<std::string> const &words);

}  // namespace FastTyping::Logic

#endif  // FASTTYPING_DATABASE_H
