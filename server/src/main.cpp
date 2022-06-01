#include <cstdlib>
#include <iostream>
#include "server.h"

int main() {
    if (!std::getenv("FASTTYPING_DB")) {  // check before database creation
        std::cerr << "specify env variable FASTTYPING_DB\n";
        std::abort();
    }
    FastTyping::Server::Server::getInstance().polling();
}