#include "dictionaryDB.h"

using FastTyping::Server::DictionariesDatabase;


int main() {
    DictionariesDatabase db;
    db.addConst("Aboba", "This is sample don't judge me");
    db.addConst("Boba", "Sztirlic nie podumal...");
    return 0;
}