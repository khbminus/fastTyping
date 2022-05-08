#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

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
#include <iostream>

namespace web {
class WebClient : public QObject {
    Q_OBJECT
    QTcpSocket *socket;

public:
    WebClient(QString ip, int port);
    ~WebClient();
    void sent(QString line);
public slots:
    void get();
};
}  // namespace web

#endif
