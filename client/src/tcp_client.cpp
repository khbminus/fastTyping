#include "include/tcp_client.h"
#include <iostream>

namespace web {
WebClient::WebClient(QString ip, int port) {
    socket = new QTcpSocket(this);
    socket->connectToHost(ip, port);

    if (socket->waitForConnected()) {
        std::cout << "Connected to " << ip.toStdString() << ":" << port
                  << std::endl;
    } else {
        std::cout << "Failed to connect to " << ip.toStdString() << ":" << port
                  << std::endl;
    }

    connect(socket, SIGNAL(readyRead()), this, SLOT(get()));
}

void WebClient::sent(QString line) {
    if (socket && socket->isOpen()) {
        QTextStream socket_stream(socket);
        socket_stream << line;
    } else {
        std::cout << "Connection failed" << std::endl;
    }
}
void WebClient::get() {
    std::cout << socket->readLine().toStdString() << std::endl;
}

WebClient::~WebClient() {
    socket->close();
    std::cout << "Disconnected" << std::endl;
}
}  // namespace web
