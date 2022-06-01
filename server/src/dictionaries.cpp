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

CorpusDictionary::CorpusDictionary(std::string const &corpus) {
    using FastTyping::Server::Database;

    Database &db = Database::get_instance();
    std::unique_lock l{db.mutex};

    pqxx::work work(db.connect);
    pqxx::result selected = work.exec("SELECT * FROM " + db.esc(corpus) +
                                      " ORDER BY random() LIMIT 2;");

    std::transform(selected.begin(), selected.end(), std::back_inserter(words),
                   [](pqxx::row const &row) -> std::string {
                       return row["WORD"].c_str();
                   });
    work.commit();
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

void add_corpus_dictionary(std::string const &name,
                           std::vector<std::string> const &words) {
    using FastTyping::Server::Database;
    Database &db = Database::get_instance();

    db.unanswered_query("CREATE TABLE IF NOT EXISTS " + db.esc(name) +
                        "(WORD TEXT UNIQUE);");
    std::string sql = "INSERT INTO " + db.esc(name) +
                      " (WORD)\n"
                      "VALUES('" +
                      boost::algorithm::join(words, "'),\n('") +
                      "') ON CONFLICT DO NOTHING;";

    std::cout << sql << std::endl;
    db.unanswered_query(sql);
}

}  // namespace FastTyping::Logic
