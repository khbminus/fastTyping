#include "database.h"
#include "dictionaryDB.h"

using namespace FastTyping::Server;

int main() {
    DictionariesStorage flusher;
    flusher.dropDictionaries();

    DictionariesStorage storage;
    storage.addDictionary("Aboba", false, "const");
    storage.addDictionary("Boba", false, "const");
    storage.addDictionary("File", false, "file");

    ConstDictionariesStorage constant;
    constant.addConst("Aboba", "This is sample don't judge me");
    constant.addConst("Boba", "Sztirlic nie podumal...");

    FileDictionariesStorage file;
    file.addFile("File", "test.txt");

    return 0;
}