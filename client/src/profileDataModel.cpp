#include "profileDataModel.h"
#include <nlohmann/json.hpp>
#include "queryTemplates.h"
#include "sonicSocket.h"

ProfileEntryData::ProfileEntryData(const nlohmann::json &body, QObject *parent)
    : QObject(parent),
      mTestsCompleted(body["gamesCnt"]),
      mAvgAccuracy(body["avgAccuracy"].get<double>() * 100.),
      mMaxWpm(body["maxWpm"]),
      mAvgWpm(body["avgWpm"]),
      mTimeTyping(
          QDateTime::fromMSecsSinceEpoch(body["sumFinishTime"].get<int>())),
      mDictionary(QString::fromStdString(body["dictName"])) {}

QString ProfileEntryData::getDictionary() const {
    return mDictionary;
}

ProfileDataModel::ProfileDataModel(QObject *parent) : QObject(parent) {
    using client::queries::get_user_name_query;
    using client::queries::getProfileQuery;
    using client::queries::getUserDictionariesQuery;
    using client::web::socket;
    using nlohmann::json;
    auto profile = json::parse(socket().query(getProfileQuery()).toStdString());
    mCommonData = new ProfileEntryData(profile["body"], this);
    auto dictionaries = json::parse(
        socket().query(getUserDictionariesQuery()).toStdString())["body"];
    std::transform(dictionaries.begin(), dictionaries.end(),
                   std::back_inserter(mDictionaries), [&](const json &body) {
                       return new ProfileEntryData(body, this);
                   });
    username = QString::fromStdString(
        json::parse(socket()
                        .query(get_user_name_query())
                        .toStdString())["body"]["userName"]
            .get<std::string>());
}
ProfileEntryData *ProfileDataModel::commonData() const {
    return mCommonData;
}
QQmlListProperty<ProfileEntryData> ProfileDataModel::dictionaries() {
    return {this, &mDictionaries};
}
const QString &ProfileDataModel::getName() const {
    return username;
}
QList<QString> ProfileDataModel::dictionariesNames() const {
    QList<QString> res;
    std::transform(
        mDictionaries.begin(), mDictionaries.end(), std::back_inserter(res),
        [&](ProfileEntryData *data) { return data->getDictionary(); });
    return res;
}

ProfileTableModel::ProfileTableModel(QObject *parent)
    : QAbstractListModel(parent) {
    using client::queries::getUserGamesQuery;
    using client::web::socket;
    using nlohmann::json;

    auto games = json::parse(
        socket().query(getUserGamesQuery(20)).toStdString())["body"];
    if (games.empty()) {
        return;
    }
    beginInsertRows(QModelIndex(), 0, games.size() - 1);
    entries.resize(games.size());
    std::transform(games.begin(), games.end(), entries.begin(),
                   [&](const json &body) -> GameEntry {
                       return {body["finishTime"], body["rawWpm"], body["wpm"],
                               body["accuracy"],
                               QString::fromStdString(body["dictName"])};
                   });
    endInsertRows();
}

int ProfileTableModel::rowCount(const QModelIndex &parent) const {
    return entries.size();
}

QVariant ProfileTableModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= rowCount(QModelIndex())) {
        return {};
    }
    const auto &entry = entries[index.row()];
    switch (role) {
        case ProfileRoles::DictionaryNameRole:
            return entry.dictionary;
        case ProfileRoles::WpmRole:
            return entry.wpm;
        case ProfileRoles::RawWpmRole:
            return entry.rawWpm;
        case ProfileRoles::AccuracyRole:
            return entry.accuracy;
        case ProfileRoles::TimeRole:
            return entry.duration;
    }
    return {};
}

QHash<int, QByteArray> ProfileTableModel::roleNames() const {
    return {{ProfileRoles::DictionaryNameRole, "dictionaryName"},
            {ProfileRoles::AccuracyRole, "accuracy"},
            {ProfileRoles::RawWpmRole, "rawWpm"},
            {ProfileRoles::TimeRole, "time"},
            {ProfileRoles::WpmRole, "wpm"}};
}