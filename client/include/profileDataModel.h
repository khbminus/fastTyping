#ifndef FASTTYPING_PROFILEDATAMODEL_H
#define FASTTYPING_PROFILEDATAMODEL_H

#include <QDateTime>
#include <QList>
#include <QObject>
#include <QQmlEngine>
#include <QQmlListProperty>
#include <nlohmann/json.hpp>

class ProfileEntryData : public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int testsCompleted MEMBER mTestsCompleted CONSTANT)
    Q_PROPERTY(QDateTime timeTyping MEMBER mTimeTyping CONSTANT)
    Q_PROPERTY(double avgWpm MEMBER mAvgWpm CONSTANT)
    Q_PROPERTY(double maxWpm MEMBER mMaxWpm CONSTANT)
    Q_PROPERTY(double avgAccuracy MEMBER mAvgAccuracy CONSTANT)
    Q_PROPERTY(
        QVariant favouriteDictionary MEMBER mFavouriteDictionary CONSTANT)
public:
    ProfileEntryData(const nlohmann::json &body, QObject *parent = nullptr);
    [[nodiscard]] QString getDictionary() const;

private:
    int mTestsCompleted = 0;
    QDateTime mTimeTyping;
    double mAvgWpm;
    double mMaxWpm;
    double mAvgAccuracy;
    QVariant mFavouriteDictionary;
    QString mDictionary;
};

class ProfileDataModel : public QObject {
    Q_OBJECT
public:
    ProfileDataModel(QObject *parent = nullptr);
    [[nodiscard]] ProfileEntryData *commonData() const;
    [[nodiscard]] QList<QString> dictionariesNames() const;
    [[nodiscard]] QQmlListProperty<ProfileEntryData> dictionaries();

private:
    ProfileEntryData *mCommonData;
    QList<ProfileEntryData *> mDictionaries;
};

#endif  // FASTTYPING_PROFILEDATAMODEL_H
