#include <fstream>
#include <sstream>
#include "database.h"
#include "dictionaries.h"
#include "dictionaryDB.h"

using namespace FastTyping::Server;
using namespace FastTyping::Logic;

int main() {
    DictionariesStorage flusher;
    flusher.dropDictionaries();

    DictionariesStorage storage;
    storage.addDictionary("Aboba", false, "const");
    storage.addDictionary("Boba", false, "const");
    storage.addDictionary("File", false, "file");
    storage.addDictionary("DLL", false, "dll");
    storage.addDictionary("English", true, "corpus");

    ConstDictionariesStorage constant;
    constant.addConst("Aboba", "This is sample don't judge me");
    constant.addConst("Boba", "Sztirlic nie podumal...");

    FileDictionariesStorage file;
    file.addFile("File", "test.txt");

    DLLDictionariesStorage dll;
    dll.addDLL("DLL", "test");

    CorpusDictionariesStorage corpus;
    corpus.addCorpus("English", "test");

    std::ifstream top1000("../common/dictionary/english_top1000.txt");

    if (top1000.is_open()) {
        storage.addDictionary("EnglishTop1000", true, "corpus");
        corpus.addCorpus("EnglishTop1000", "EnglishTop1000");

        std::stringstream raw;
        raw << top1000.rdbuf();
        add_corpus_dictionary("EnglishTop1000", to_words(raw.str()));

    }

    add_corpus_dictionary("test", {"name", "main", "moon"});

    return 0;
}