#ifndef FASTTYPING_PROFILEDATAMODEL_H
#define FASTTYPING_PROFILEDATAMODEL_H

#include <QAbstractTableModel>
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

struct GameEntry {
    int duration;
    double rawWpm;
    double wpm;
    double accuracy;
    QString dictionary;
};

class ProfileTableModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum ProfileRoles {
        DictionaryNameRole = Qt::UserRole + 1,
        WpmRole,
        RawWpmRole,
        AccuracyRole,
        TimeRole,
    };
    ProfileTableModel(QObject *parent = nullptr);

    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index,
                                int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

private:
    QList<GameEntry> entries;
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
