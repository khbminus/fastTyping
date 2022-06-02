#include "WpmChartModel.h"

namespace FastTyping::Charts {
int WpmChartModel::rowCount(const QModelIndex &parent) const {
    assert(allPresses.size() == correctPresses.size());
    assert(correctPresses.size() == incorrectPresses.size());
    return static_cast<int>(allPresses.size());
}

WpmChartModel::WpmChartModel(QObject *parent) : QAbstractTableModel(parent) {}

QVariant WpmChartModel::data(const QModelIndex &index, int role) const {
    if (index.row() >= rowCount(QModelIndex()) || index.row() < 0 ||
        index.column() < 0 || index.column() >= 4 || role != Qt::DisplayRole) {
        return {};
    }
    switch (index.column()) {
        case 0:
            return index.row() + 1;
        case 1:
            return allPresses[index.row()] * 15;
        case 2:
            return correctPresses[index.row()] * 15;
        case 3:
            return incorrectPresses[index.row()];
    }
    return {};
}

void WpmChartModel::correctSymbol() {
    auto time = getTimeAndEnsure();
    allPresses[time]++;
    correctPresses[time]++;
}

void WpmChartModel::errorSymbol(QChar have, QChar shouldBe) {
    auto time = getTimeAndEnsure();
    assert(allPresses.size() == correctPresses.size());
    assert(correctPresses.size() == incorrectPresses.size());
    allPresses[time]++;
    incorrectPresses[time]++;
    qDebug() << "should be" << shouldBe << "but have" << have;
    incorrectPressedWithButtons << std::make_pair(shouldBe, have);
}

void WpmChartModel::backspace() {
#ifdef false
    auto time = getTimeAndEnsure();
    allPresses[time]++;
#endif
}

int WpmChartModel::getTimeAndEnsure() {
    using namespace std::literals;
    auto time = (clock.now() - startTime) / 1s;
    assert(allPresses.size() == correctPresses.size());
    assert(correctPresses.size() == incorrectPresses.size());
    if (time >= allPresses.size()) {
        beginInsertRows(QModelIndex(), allPresses.size(), time);
        allPresses.resize(time + 1);
        incorrectPresses.resize(time + 1);
        correctPresses.resize(time + 1);
        endInsertRows();
    }
    assert(time < allPresses.size());
    return time;
}

void WpmChartModel::startGame() {
    startTime = clock.now();
}

int WpmChartModel::getMaxWPM() {
    // supposed, that rawWPM >= correctWPM
    return *std::max_element(allPresses.begin(), allPresses.end()) * 15;
}

int WpmChartModel::getMaxErrors() {
    return *std::max_element(incorrectPresses.begin(), incorrectPresses.end());
}

QList<std::pair<QChar, QChar>> WpmChartModel::getAllErrors() const {
    return incorrectPressedWithButtons;
}

}  // namespace FastTyping::Charts