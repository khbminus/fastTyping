#ifndef FASTTYPING_PROFILEDATAMODEL_H
#define FASTTYPING_PROFILEDATAMODEL_H

#include <QDateTime>
#include <QList>
#include <QObject>
#include <QQmlEngine>

class ProfileDataModel /*: public QObject*/ {
    //    Q_OBJECT
    //    Q_PROPERTY(QString userName READ userName CONSTANT)
    //    Q_PROPERTY(QStringList playedDictionaries READ playedDictionaries
    //    CONSTANT) Q_PROPERTY(int testsCompleted READ testsCompleted CONSTANT)
    //    Q_PROPERTY(QDateTime timeTyping READ timeTyping CONSTANT)
    //    Q_PROPERTY(double avgWpm READ avgWpm CONSTANT)
    //    Q_PROPERTY(double avgAccuracy READ avgAccuracy CONSTANT)
    //    Q_PROPERTY(QString favouriteDictionary READ favouriteDictionary
    //    CONSTANT)
public:
    ProfileDataModel(QObject *parent = nullptr);
    QString userName() const;
    QList<QString> playedDictionaries() const;
    int testsCompleted() const;
    QDateTime timeTyping() const;
    double avgWpm() const;
    double maxWpm() const;
    double avgAccuracy() const;
    QString favouriteDictionary() const;

private:
};

#endif  // FASTTYPING_PROFILEDATAMODEL_H
