#ifndef SOCKET_WRAPPER_H
#define SOCKET_WRAPPER_H

#include <QAbstractSocket>
#include <QDebug>
#include <QEventLoop>
#include <QString>
#include <QTcpSocket>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>

namespace client::web {
class QuerySender : public QObject {
    Q_OBJECT
public:
    void send(QString line);
signals:
    void send_line(QString line);
};

enum class ResponseType { blocking, sync, async };

class ResponseHandler {
public:
    virtual ResponseType type(QString const &response) const = 0;
    virtual void handle(QString const &response) = 0;
    virtual ~ResponseHandler() {}
};

class TestHandler final : public ResponseHandler {
    ResponseType type(QString const &response) const override {
        return ResponseType::blocking;
    }
    void handle(QString const &response) override {
        std::cout << "async processing(" << response.toStdString() << ")"
                  << std::endl;
    }
};

/*
 * works in 3 different ways:
 * (1) - just sent an async query,
 *      after receiving response it would be operated by response handler
 * (2) -  sending query, after receiving response it would be pushed into queue.
 *      `get_response` returns head of query (Maybe there should be more
 * complicated DS, than queue) (3) - send blocking query and wait for response.
 */

class SocketWrapper : public QObject {
    Q_OBJECT
    std::optional<QTcpSocket *> socket_wrap = std::nullopt;
    std::optional<QEventLoop *> loop_wrap = std::nullopt;
    ResponseHandler *handler;

    std::mutex init_mutex;
    std::condition_variable wait_for_init;

    std::queue<QString> responses;
    std::mutex response_mutex;
    std::condition_variable wait_for_response;

    std::optional<QString> blocking_query = std::nullopt;
    std::mutex blocking_query_mutex;
    std::condition_variable wait_for_blocking_query;
    QuerySender sender;

public:
    SocketWrapper(QString ip, short port, ResponseHandler *a_handler);
    ~SocketWrapper();
    // for async queries
    void send(QString const &line);
    // could cause race condition, be careful
    QString get_response();
    // prefer over send + get_respopnse to avoid race condition
    QString query(QString const &line);
};
}  // namespace client::web

#endif  // TEMPORARY_H
