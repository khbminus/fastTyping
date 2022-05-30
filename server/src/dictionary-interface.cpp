#include "dictionaryDB.h"

using namespace FastTyping::Server;

int main() {
    ConstDictionariesStorage db;
    db.addConst("Aboba", "This is sample don't judge me");
    db.addConst("Boba", "Sztirlic nie podumal...");
    return 0;
}