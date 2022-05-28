#include "textScreen.h"
#include "queryTemplates.h"
#include "responseHandler.h"
#include "sonicSocket.h"

namespace FastTyping::TextScreen {
TextListModel::TextListModel(const QString &words, QObject *parent)
    : QAbstractListModel(parent), statesTimer(new QTimer(this)) {
    beginInsertRows(QModelIndex(), 0, words.size());
    for (const auto &x : words) {
        line << new ScreenCharPimpl(x, this);
    }
    for (int i = 0; i < words.size(); i++) {
        players << new ProgressListModel(this);
    }
    endInsertRows();

    QObject::connect(statesTimer, &QTimer::timeout, this,
                     &TextListModel::updateState);
    QObject::connect(&client::responses::handler(),
                     &client::responses::APIHandler::stateUpdated, this,
                     &TextListModel::onUpdated);
    statesTimer->start(200);
}

int TextListModel::rowCount(const QModelIndex &) const {
    return static_cast<int>(line.size());
}

QVariant TextListModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= rowCount()) {
        return {};
    }
    switch (role) {
        case CORRECTNESS_ROLE:
            return line[index.row()]->isCorrect();
        case CURSOR_ROLE:
            return (index.row() == currentCursor);
        case LETTER_ROLE:
            return line[index.row()]->getChar();
        case PLAYERS_ROLE:
            return QVariant::fromValue(players.at(index.row()));
    }
    return {};
}

QHash<int, QByteArray> TextListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[CORRECTNESS_ROLE] = "isCorrect";
    roles[CURSOR_ROLE] = "isCursor";
    roles[LETTER_ROLE] = "letter";
    roles[PLAYERS_ROLE] = "players";
    return roles;
}

void TextListModel::setCorrectnessOfChar(int position, bool value) {
    qDebug() << "setted" << position << value;
    if (position >= line.size()) {
        return;
    }
    line[position]->setCorrectness(value);
    dataChanged(createIndex(position, 0), createIndex(position, 0),
                {CORRECTNESS_ROLE});
}

void TextListModel::onCorrectChar(int position) {
    setCorrectnessOfChar(position, true);
}
void TextListModel::onWrongChar(int position) {
    setCorrectnessOfChar(position, false);
}

void TextListModel::onMove(const QString & /*buffer*/, int position) {
    std::swap(position, currentCursor);
    dataChanged(createIndex(position, 0), createIndex(position, 0),
                {CURSOR_ROLE});
    dataChanged(createIndex(currentCursor, 0), createIndex(currentCursor, 0),
                {CURSOR_ROLE});
    emit cursorMoved(currentCursor);
}

ProgressListModel::ProgressListModel(QObject *parent)
    : QAbstractListModel(parent), players(static_cast<qsizetype>(0)) {}

int ProgressListModel::rowCount(const QModelIndex & /*parent*/) const {
    return players.size();
}

void ProgressListModel::clear() {
    if (players.empty())
        return;
    beginRemoveRows(QModelIndex(), 0, players.size() - 1);
    players.clear();
    endRemoveRows();
}

QVariant ProgressListModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= players.size()) {
        return {};
    }
    if (role == COLOR_ROLE) {
        return players[index.row()].getColor();
    } else if (role == OPACITY_ROLE) {
        return players[index.row()].getOpacity();
    } else if (role == USERNAME_ROLE) {
        return QString::number(players[index.row()].getId());
    }
    return {};
}

void ProgressListModel::addPlayer(const Player &player) {
    beginInsertRows(QModelIndex(), players.size(), players.size());
    players << player;
    endInsertRows();
}

void ProgressListModel::showPlayer(const Player &player) {
    int playerIndex = players.indexOf(player);
    if (playerIndex != -1) {
        players[playerIndex].show();
        dataChanged(createIndex(playerIndex, 0), createIndex(playerIndex, 0),
                    {OPACITY_ROLE});
    }
}

void TextListModel::updateState() {
    using client::queries::getStatesQuery;
    using client::web::socket;
    socket().send(getStatesQuery());
}

void TextListModel::onUpdated(const nlohmann::json &query) {
    const auto &users = query["body"]["userStates"];
    for (auto *list : players) {
        list->clear();
        for (const auto &player : users) {
            list->addPlayer(Player(player["id"].get<int>()));
        }
    }

    for (const auto &player : users) {
        int pos = player["symbolsTyped"].get<int>();
        if (pos < players.size()) {
            players[pos]->showPlayer(Player(player["id"].get<int>()));
        }
    }
    dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, 0),
                {PLAYERS_ROLE});
}

QHash<int, QByteArray> ProgressListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[COLOR_ROLE] = "color";
    roles[OPACITY_ROLE] = "opacity";
    roles[USERNAME_ROLE] = "username";
    return roles;
}
bool Player::operator==(const Player &rhs) const {
    return userId == rhs.userId;
}
}  // namespace FastTyping::TextScreen