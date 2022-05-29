#include <boost/asio.hpp>
#include <iostream>
#include "server.h"

int main() {
    boost::asio::io_context context;
    FastTyping::Server::Server::getInstance(context).polling();
    context.run();
}