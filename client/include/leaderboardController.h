#ifndef FASTTYPING_LEADERBOARDCONTROLLER_H
#define FASTTYPING_LEADERBOARDCONTROLLER_H
#include <QAbstractTableModel>
#include <QTableView>

struct DictionaryStatistics {
    int userId;
    double mMaxWpm;
    double mAvgWpm;
    double mAvgAccuracy;
    int mTimeTyping;
    int mTestsCompleted;
    QString name;
};

class LeaderboardTableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit LeaderboardTableModel(QString dictionaryName,
                                   QObject *parent = nullptr);

    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index,
                                int role = Qt::DisplayRole) const override;
    [[nodiscard]] QVariant headerData(int section,
                                      Qt::Orientation orientation,
                                      int role) const override;

private:
    QList<DictionaryStatistics> rows;
};

class LeaderboardController : public QObject {
    Q_OBJECT
public:
    explicit LeaderboardController(QObject *parent = nullptr);

    [[nodiscard]] QList<std::pair<QTableView *, QString>> getDictionariesTabs()
        const;

private:
    QList<QString> dictionariesNames;
    QList<LeaderboardTableModel *> models;
};

#endif  // FASTTYPING_LEADERBOARDCONTROLLER_H
