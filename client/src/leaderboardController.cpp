#include "leaderboardController.h"
#include <QHeaderView>
#include <nlohmann/json.hpp>
#include "queryTemplates.h"
#include "sonicSocket.h"

LeaderboardController::LeaderboardController(QObject *parent)
    : QObject(parent) {
    using client::queries::get_dictionaries_query;
    using client::web::socket;
    using nlohmann::json;

    auto dictionaries = json::parse(
        socket().query(get_dictionaries_query()).toStdString())["body"]["list"];

    dictionariesNames.resize(dictionaries.size());
    std::transform(dictionaries.begin(), dictionaries.end(),
                   dictionariesNames.begin(), &QString::fromStdString);

    models.resize(dictionaries.size());
    std::transform(dictionariesNames.begin(), dictionariesNames.end(),
                   models.begin(),
                   [&](const QString &name) -> LeaderboardTableModel * {
                       return new LeaderboardTableModel(name, this);
                   });
}
QList<std::pair<QTableView *, QString>>
LeaderboardController::getDictionariesTabs() const {
    QList<std::pair<QTableView *, QString>> res(dictionariesNames.size());
    for (qsizetype i = 0; i < res.size(); i++) {
        res[i] = {new QTableView(), dictionariesNames[i]};
        res[i].first->setModel(models[i]);
        res[i].first->verticalHeader()->hide();
        res[i].first->setEditTriggers(QAbstractItemView::NoEditTriggers);
        res[i].first->setFocusPolicy(Qt::NoFocus);
        res[i].first->setSelectionMode(QAbstractItemView::NoSelection);
    }
    return res;
}

LeaderboardTableModel::LeaderboardTableModel(QString dictionaryName,
                                             QObject *parent)
    : QAbstractTableModel(parent) {
    using client::queries::getDictionaryStatsQuery;
    using client::web::socket;
    using nlohmann::json;

    auto body = json::parse(socket()
                                .query(getDictionaryStatsQuery(dictionaryName))
                                .toStdString())["body"];
    if (body.empty()) {
        return;
    }
    std::sort(body.begin(), body.end(), [&](const json &a, const json &b) {
        return a["maxWpm"].get<double>() < b["maxWpm"].get<double>();
    });
    beginInsertRows(QModelIndex(), 0, body.size() - 1);
    rows.resize(body.size());
    std::transform(body.begin(), body.end(), rows.begin(),
                   [&](const json &body) -> DictionaryStatistics {
                       return {body["userId"],        body["maxWpm"],
                               body["avgWpm"],        body["avgAccuracy"],
                               body["sumFinishTime"], body["gamesCnt"]};
                   });
    endInsertRows();
}

int LeaderboardTableModel::rowCount(const QModelIndex &parent) const {
    return rows.size();
}
int LeaderboardTableModel::columnCount(const QModelIndex &parent) const {
    return 6;
}
QVariant LeaderboardTableModel::data(const QModelIndex &index, int role) const {
    if (role != Qt::DisplayRole) {
        return {};
    }
    const auto &entry = rows[index.row()];
    switch (index.column()) {
        case 0:
            return entry.userId;
        case 1:
            return QString::number(entry.mMaxWpm, 'g', 2);
        case 2:
            return QString::number(entry.mAvgWpm, 'g', 2);
        case 3:
            return QString::number(entry.mAvgAccuracy * 100, 'g', 2) + "%";
        case 4:
            return QString::number(entry.mTimeTyping) + " seconds";
        case 5:
            return entry.mTestsCompleted;
    }
    return {};
}

QVariant LeaderboardTableModel::headerData(int section,
                                           Qt::Orientation orientation,
                                           int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return QString("User ID");
            case 1:
                return QString("Max WPM");
            case 2:
                return QString("Avg WPM");
            case 3:
                return QString("Accuracy");
            case 4:
                return QString("Time typing");
            case 5:
                return QString("Games completed");
        }
    }
    return {};
}