#ifndef FASTTYPING_WPMCHARTMODEL_H
#define FASTTYPING_WPMCHARTMODEL_H
#include <QAbstractTableModel>
#include <chrono>

namespace FastTyping::Charts {
Q_NAMESPACE

class WpmChartModel : public QAbstractTableModel {
    Q_OBJECT
public:
    WpmChartModel(QObject *parent = nullptr);

    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent) const override {
        return 4;
    }
    [[nodiscard]] QVariant data(const QModelIndex &index,
                                int role = Qt::DisplayRole) const override;

    [[nodiscard]] QList<std::pair<QChar, QChar>> getAllErrors() const;

    void correctSymbol();
    void errorSymbol(QChar shouldBe, QChar have);
    void backspace();

    void startGame();

    int getMaxWPM();
    int getMaxErrors();

private:
    int getTimeAndEnsure();

    QList<int> correctPresses;
    QList<int> incorrectPresses;
    QList<int> allPresses;
    QList<std::pair<QChar, QChar>> incorrectPressedWithButtons;
    QString wholeText;
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock clock;
public slots:
};

}  // namespace FastTyping::Charts

#endif  // FASTTYPING_WPMCHARTMODEL_H
