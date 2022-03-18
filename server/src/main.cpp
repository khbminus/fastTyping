#include "server.h"
#include <boost/asio.hpp>
#include <iomanip>
#include <iostream>


int main() {
    FastTyping::Server::Server::getInstance().polling();
}