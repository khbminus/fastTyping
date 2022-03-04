#include "common/include/json.hpp"
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
    boost::asio::io_context ioContext;
    tcp::acceptor acceptor(ioContext, tcp::endpoint(tcp::v4(), 1245 /* FIXME */));

    std::cout << "Listening at " << acceptor.local_endpoint() << std::endl;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        tcp::socket sock = acceptor.accept();
        std::thread([s = std::move(sock)]() mutable {
            std::cout << "Accepted connection " << s.remote_endpoint() << "-->" << s.local_endpoint() << std::endl;
            auto remote_endpoint = s.remote_endpoint();
            auto local_endpoint = s.local_endpoint();
            tcp::iostream client(std::move(s));
            try {
            while (client) {// we can't use exceptions here
                json query_header, query_body;
                if (!(client >> query_header >> query_body)) {
                    break;
                }
                std::cout << "Got new query. Type: " << query_header["type"] << std::endl
                          << std::setw(4) << query_header << std::endl;
                std::cout << "Body: " << query_body << std::endl;
                if (query_header["type"] == "exit")
                    break;
            }
            } catch (nlohmann::detail::parse_error& e) {
                std::cerr << e.what() << std::endl;
                sendError(client, e);
            }

            std::cout << "Closed connection " << remote_endpoint << "-->" << local_endpoint << std::endl;
        }).detach();
    }
#pragma clang diagnostic pop
}