#include "dictionaries.h"
#include <algorithm>
#include <boost/dll/import.hpp>
#include <boost/version.hpp>
#include <cctype>
#include <iostream>
#include <pqxx/pqxx>
#include <sstream>
#include "boost/algorithm/string.hpp"
#include "database.h"
#include "dictionaries_api.h"

#if BOOST_VERSION >= 107600
#define boost_dll_import_symbol ::boost::dll::import_symbol
#else
#define boost_dll_import_symbol ::boost::dll::import
#endif

namespace FastTyping::Logic {

std::vector<std::string> to_words(std::string raw) {
    std::vector<std::string> words;
    boost::split(words, raw, boost::is_space());
    std::for_each(words.begin(), words.end(), [](std::string &val) {
        boost::trim_if(val, boost::is_space());
    });
    words.erase(
        std::remove_if(words.begin(), words.end(),
                       [](std::string const &val) { return val.empty(); }),
        words.end());
    return words;
}

FileDictionary::FileDictionary(std::string const &filename) : file(filename) {
    std::stringstream raw;
    if (!file.is_open()) {
        words = {"Can", "not", "load", "dictionary"};
    } else {
        raw << file.rdbuf();
        words = to_words(raw.str());
    }
}

[[nodiscard]] std::string FileDictionary::getWord(int index) const {
    return words[index];
}

[[nodiscard]] std::size_t FileDictionary::getWordCount() const {
    return words.size();
}
[[nodiscard]] std::vector<std::string> FileDictionary::getLine(
    int index) const {
    return words;
}
[[nodiscard]] std::size_t FileDictionary::getLinesCount() const {
    return 1;
}
[[nodiscard]] std::size_t FileDictionary::getPrefixSize(int pos) const {
    std::size_t result = 0;
    for (int ind = 0; ind < pos; ind++) {
        result += words[ind].size() + 1;
    }
    return result;
}

CorpusDictionary::CorpusDictionary(std::string const &corpus,
                                   bool adapt,
                                   int user_id) {
    using FastTyping::Server::Database;
    using FastTyping::Server::MistakesStorage;

    std::cout << "in constructor" << std::endl;
    std::string sql;

    Database &db = Database::get_instance();

    if (!adapt) {
        sql =
            "SELECT * FROM " + db.esc(corpus) + " ORDER BY random() LIMIT 20;";
    } else {
        MistakesStorage mistakesStorage;

        std::vector<std::pair<char, char>> mistakes =
            mistakesStorage.getTopMistakes(user_id, 10, "qwerty");

        std::vector<std::string> mistakesStrings;

        std::transform(mistakes.begin(), mistakes.end(),
                       std::back_inserter(mistakesStrings),
                       [&](std::pair<char, char> symbols) {
                           auto [a, b] = symbols;
                           return db.esc(std::string{a, b});
                       });

        std::string condition =
            "WORD LIKE '%" +
            boost::join(mistakesStrings, "%'\n OR WORD LIKE '%") + "%' ";

        sql = "SELECT * FROM " + db.esc(corpus) + " WHERE " + condition +
              " ORDER BY random() LIMIT 20;";
    }

    std::unique_lock l{db.mutex};
    pqxx::work work(db.connect);

    pqxx::result selected = work.exec(sql);

    std::transform(selected.begin(), selected.end(), std::back_inserter(words),
                   [](pqxx::row const &row) -> std::string {
                       return row["WORD"].c_str();
                   });
    work.commit();

    if (words.empty()) {
        words = {"No", "typos", "registered"};
    }
}

[[nodiscard]] std::string CorpusDictionary::getWord(int index) const {
    return words[index];
}

[[nodiscard]] std::size_t CorpusDictionary::getWordCount() const {
    return words.size();
}
[[nodiscard]] std::vector<std::string> CorpusDictionary::getLine(
    int index) const {
    return words;
}
[[nodiscard]] std::size_t CorpusDictionary::getLinesCount() const {
    return 1;
}

DLLDictionary::DLLDictionary(std::string const &filename) {
    boost::shared_ptr<dictionary_plugin> plugin =
        boost_dll_import_symbol<dictionary_plugin>(
            filename, "dictionary", boost::dll::load_mode::append_decorations);
    words = plugin->words();
}

[[nodiscard]] std::string DLLDictionary::getWord(int index) const {
    return words[index];
}

[[nodiscard]] std::size_t DLLDictionary::getWordCount() const {
    return words.size();
}
[[nodiscard]] std::vector<std::string> DLLDictionary::getLine(int index) const {
    return words;
}
[[nodiscard]] std::size_t DLLDictionary::getLinesCount() const {
    return 1;
}

[[nodiscard]] std::size_t DLLDictionary::getPrefixSize(int pos) const {
    std::size_t result = 0;
    for (int ind = 0; ind < pos; ind++) {
        result += words[ind].size() + 1;
    }
    return result;
}

void add_corpus_dictionary(std::string const &name,
                           std::vector<std::string> words) {
    using FastTyping::Server::Database;
    Database &db = Database::get_instance();

    db.unanswered_query("CREATE TABLE IF NOT EXISTS " + db.esc(name) +
                        "(WORD TEXT UNIQUE);");

    std::transform(words.begin(), words.end(), words.begin(),
                   [&](std::string const &line) { return db.esc(line); });

    std::string sql = "INSERT INTO " + db.esc(name) +
                      " (WORD)\n"
                      "VALUES('" +
                      boost::algorithm::join(words, "'),\n('") +
                      "') ON CONFLICT DO NOTHING;";

    db.unanswered_query(sql);
}

}  // namespace FastTyping::Logic
