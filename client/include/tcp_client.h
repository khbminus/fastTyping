#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <iostream>
#include <memory>

#include <QAbstractSocket>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QHostAddress>
#include <QMainWindow>
#include <QMessageBox>
#include <QMetaType>
#include <QStandardPaths>
#include <QString>
#include <QTcpSocket>

#include "client/include/gameClientFwd.h"
#include "common/include/json.hpp"

using json = nlohmann::json;

namespace game::client {
    struct Query {
        virtual void process(GameClient *) = 0;
        virtual ~Query(){};
    };

    struct CheckQuery : Query {
    private:
        json status;

    public:
        CheckQuery(json status_) : status(status_) {}
        void process(GameClient *) override;
    };

    struct EmptyQuery : Query {
        void process(GameClient *) override {}
    };

    std::unique_ptr<Query> ParseQuery(QString);
}// namespace game::client

namespace web {
    class WebClient : public QObject {
        Q_OBJECT
        QTcpSocket *socket;
        QString buffer;
        game::client::GameClient *manager;

    public:
        WebClient(QString ip, int port, game::client::GameClient *handler);
        ~WebClient();
        void sent(QString line);
    public slots:
        void get();
    };
}// namespace web

#endif
