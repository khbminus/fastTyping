#include "common/include/json.hpp"
#include "server.h"
#include <boost/asio.hpp>
#include <iomanip>
#include <iostream>

using boost::asio::ip::tcp;
using nlohmann::json;


int main() {
    FastTyping::Server::Server::getInstance().polling();
}