#include "common/include/json.hpp"
#include "server.h"
#include <boost/asio.hpp>
#include <iomanip>
#include <iostream>

using boost::asio::ip::tcp;
using nlohmann::json;

void sendError(std::ostream& os, std::exception& e) {
    json errorJ_header, errorJ_body;
    errorJ_header["type"] = "error";
    errorJ_body["what"] = e.what();
    os << errorJ_header << "\n" << errorJ_body << "\n";
}

int main() {
    FastTyping::Server::Server::getInstance().polling();
}