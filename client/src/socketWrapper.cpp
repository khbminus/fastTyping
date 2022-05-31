#include "socketWrapper.h"
#include <iostream>
#include "errorHandler.h"

namespace client::web {
void QuerySender::send(QString line) {
    emit send_line(line);
}

SocketWrapper::SocketWrapper(QString ip, short port, ResponseHandler *a_handler)
    : handler(a_handler) {
    std::thread test([this, ip, port]() {
        init_mutex.lock();
        QEventLoop socket_thread;
        QTcpSocket *socket = new QTcpSocket(&socket_thread);
        socket->connectToHost(ip, port);

        QString buffer;

        if (socket->waitForConnected()) {
            qInfo() << "connected to host(" << ip << ":" << port << ") \n";
        } else {
            qCritical() << "failed to connect to host\n";
            exit(1);
        }

        QObject::connect(&sender, &QuerySender::send_line, &socket_thread,
                         [socket](QString line) {
                             if (socket && socket->isOpen()) {
                                 // qDebug() << "send:  " << line;
                                 QTextStream socket_stream(socket);
                                 socket_stream << line;
                             } else {
                                 qDebug() << "failed to send\n";
                             }
                         });

        QObject::connect(
            socket, &QTcpSocket::readyRead, &socket_thread,
            [this, socket, &buffer]() {
                QString received = socket->readLine();
                buffer += received;
                // qDebug() << "raw: " << received;

                // I think that `.split()` is amgisious - Qt should call it on
                // every line, that ends with '\n'
                if (!buffer.isEmpty() && buffer.back() == '\n') {
                    QStringList list =
                        buffer.split(QLatin1Char('\n'), Qt::SkipEmptyParts);

                    for (std::size_t i = 0; i < list.size(); i++) {
                        QString response = list.at(i);

                        // qDebug() << "received: " << response;

                        ResponseType type = handler->type(response);
                        if (type == ResponseType::async) {
                            handler->handle(response);
                        } else if (type == ResponseType::sync) {
                            std::unique_lock response_lock{response_mutex};
                            responses.push(response);
                            wait_for_response.notify_one();
                        } else if (type == ResponseType::blocking) {
                            std::unique_lock blocking_lock{
                                blocking_query_mutex};
                            blocking_query = response;
                            wait_for_blocking_query.notify_one();
                        }
                    }
                    buffer.clear();
                }
            });

        QObject::connect(socket, &QTcpSocket::disconnected, &socket_thread,
                         [&socket_thread]() {
                             qDebug() << "connection lost\n";
                             socket_thread.quit();
                             exit(1);
                         });

        socket_wrap = socket;
        loop_wrap = &socket_thread;

        init_mutex.unlock();
        wait_for_init.notify_one();
        socket_thread.exec();

        std::unique_lock l{init_mutex};
        socket->deleteLater();
        loop_wrap.reset();
    });

    test.detach();

    std::unique_lock l{init_mutex};

    while (!socket_wrap) {
        wait_for_init.wait(l);
    }
    // cppcheck-suppress knownConditionTrueFalse
    if (socket_wrap == nullptr) {
        exit(0);
    }
}

void SocketWrapper::send(QString const &line) {
    sender.send(line);
}

SocketWrapper::~SocketWrapper() {
    std::unique_lock l{init_mutex};

    if (loop_wrap && (*loop_wrap)->isRunning()) {
        (*loop_wrap)->quit();
    }

    qDebug() << "disconnected";
}

QString SocketWrapper::get_response() {
    std::unique_lock l{response_mutex};
    while (responses.empty()) {
        wait_for_response.wait(l);
    }
    QString result = std::move(responses.back());
    responses.pop();
    return result;
}

QString SocketWrapper::query(QString const &line) {
    std::unique_lock l{blocking_query_mutex};
    send(line);
    while (!blocking_query) {
        wait_for_blocking_query.wait(l);
    }
    QString result = std::move(*blocking_query);
    blocking_query = std::nullopt;
    return result;
}
}  // namespace client::web
