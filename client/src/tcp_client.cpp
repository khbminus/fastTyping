#include "client/include/tcp_client.h"
#include "client/include/gameClient.h"
#include <iostream>

namespace game::client {
    std::unique_ptr<Query> ParseQuery(QString raw) {
        json query = json::parse(raw.toStdString());

        if (query["header"]["type"].get<std::string>() == "checkResult") {
            return std::make_unique<CheckQuery>(query["body"]);
        } else {
            return std::make_unique<EmptyQuery>();
        }
    }

    void CheckQuery::process(GameClient *client) {
        //client->mIsEnded = status["isEnd"].get<bool>();
        client->mIsCorrect = status["isPrefixCorrect"].get<bool>();
    }
}// namespace game::client

namespace web {
    WebClient::WebClient(QString ip, int port, game::client::GameClient *handler) : manager(handler) {
        return;
        socket = new QTcpSocket(this);
        socket->connectToHost(ip, port);


        if (socket->waitForConnected()) {
            std::cout << "Connected to " << ip.toStdString() << ":" << port << std::endl;
        } else {
            std::cout << "Failed to connect to " << ip.toStdString() << ":" << port << std::endl;
        }

        connect(socket, SIGNAL(readyRead()), this, SLOT(get()));
    }

    void WebClient::sent(QString line) {
        return;
        if (socket && socket->isOpen()) {
            //std::cout << "sent(" << line.toStdString() << ")sent" << std::endl;
            QTextStream socket_stream(socket);
            socket_stream << line;
        } else {
            std::cout << "Connection failed" << std::endl;
        }
    }

    void WebClient::get() {
        return;
        buffer += socket->readLine();

        //std::cout << "buffer(" << buffer.toStdString() << ")buffer" << std::endl;

        if (!buffer.isEmpty() && buffer.back() == '\n') {
            QStringList list = buffer.split(QLatin1Char('\n'), Qt::SkipEmptyParts);
            for (std::size_t i = 0; i < list.size(); i++) {
                std::cout << "get(" << list.at(i).toStdString() << ")get" << std::endl;
                auto query = game::client::ParseQuery(list.at(i));
                query->process(manager);
            }
            buffer.clear();
        }
    }

    WebClient::~WebClient() {
        return;
        socket->close();
        std::cout << "Disconnected" << std::endl;
    }
}// namespace web
