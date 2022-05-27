#include "textScreen.h"

namespace FastTyping::TextScreen {
TextListModel::TextListModel(const QString &words, QObject *parent)
    : QAbstractListModel(parent) {
    beginInsertRows(QModelIndex(), 0, words.size());
    for (const auto &x : words) {
        line << new ScreenCharPimpl(x, this);
    }
    endInsertRows();
}

int TextListModel::rowCount(const QModelIndex &) const {
    return static_cast<int>(line.size());
}

QVariant TextListModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() > rowCount()) {
        return {};
    }
    switch (role) {
        case CORRECTNESS_ROLE:
            return line[index.row()]->isCorrect();
        case CURSOR_ROLE:
            return (index.row() == currentCursor);
        case LETTER_ROLE:
            return line[index.row()]->getChar();
    }
    return {};
}

QHash<int, QByteArray> TextListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[CORRECTNESS_ROLE] = "isCorrect";
    roles[CURSOR_ROLE] = "isCursor";
    roles[LETTER_ROLE] = "letter";
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

void TextListModel::onMove(const QString &buffer, int position) {
    std::swap(position, currentCursor);
    dataChanged(createIndex(position, 0), createIndex(position, 0),
                {CURSOR_ROLE});
    dataChanged(createIndex(currentCursor, 0), createIndex(currentCursor, 0),
                {CURSOR_ROLE});
    emit cursorMoved(currentCursor);
}

}  // namespace FastTyping::TextScreen