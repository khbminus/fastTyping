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
}
ProfileEntryData *ProfileDataModel::commonData() const {
    return mCommonData;
}
QQmlListProperty<ProfileEntryData> ProfileDataModel::dictionaries() {
    return {this, &mDictionaries};
}
QList<QString> ProfileDataModel::dictionariesNames() const {
    QList<QString> res;
    std::transform(
        mDictionaries.begin(), mDictionaries.end(), std::back_inserter(res),
        [&](ProfileEntryData *data) { return data->getDictionary(); });
    return res;
}