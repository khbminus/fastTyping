#ifndef FASTTYPING_TEXTSCREEN_H
#define FASTTYPING_TEXTSCREEN_H

#include <QAbstractListModel>
#include <QColor>
#include <QObject>
#include <QQmlEngine>
#include <QTimer>
#include <nlohmann/json.hpp>

namespace FastTyping::TextScreen {
Q_NAMESPACE
class ScreenCharPimpl : public QObject {
    Q_OBJECT
public:
    explicit ScreenCharPimpl(QChar c, QObject *parent = nullptr)
        : letter(c), QObject(parent) {}

    [[nodiscard]] QString getChar() const {
        return letter;
    }

    [[nodiscard]] bool isCorrect() const {
        return mIsCorrect;
    }
    void setCorrectness(bool newValue) {
        mIsCorrect = newValue;
    }

private:
    QChar letter;
    bool mIsCorrect = true;
};

class Player {
public:
    Player() = default;
    explicit Player(int userId) : userId(userId) {}
    [[nodiscard]] QColor getColor() const {
        return color;
    }
    [[nodiscard]] int getId() const {
        return userId;
    }

    [[nodiscard]] double getOpacity() const {
        return opacity;
    }

    void show() {
        opacity = 1;
    }
    bool operator==(const Player &rhs) const;

private:
    int userId;
    QColor color = "green";
    double opacity = 0;
};

class ProgressListModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum TextRoles {
        COLOR_ROLE = Qt::UserRole + 1,
        OPACITY_ROLE,
        USERNAME_ROLE
    };
    explicit ProgressListModel(QObject *parent = nullptr);
    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index,
                                int role = Qt::DisplayRole) const override;

    void addPlayer(const Player &player);
    void showPlayer(const Player &player);
    void clear();
    void print() {
        QString res;
        for (const auto &x : players) {
            res += "{" + x.getColor().name() + ", " +
                   QString::number(x.getId()) + "}";
        }
        qDebug() << res;
    }

protected:
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

private:
    QList<Player> players;
};
class TextListModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum TextRoles {
        CORRECTNESS_ROLE = Qt::UserRole + 1,
        CURSOR_ROLE,
        LETTER_ROLE,
        PLAYERS_ROLE,
    };
    explicit TextListModel(const QString &words, QObject *parent = nullptr);
    [[nodiscard]] int rowCount(
        const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index,
                                int role = Qt::DisplayRole) const override;

signals:
    void cursorMoved(QVariant position);
public slots:
    void onWrongChar(int position);
    void onCorrectChar(int position);
    void onMove(const QString &buffer, int position);
    void onUpdated(const nlohmann::json &);

protected:
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;
private slots:
    void updateState();

private:
    void setCorrectnessOfChar(int position, bool value);

    int currentCursor = 0;
    QList<ScreenCharPimpl *> line;
    QList<ProgressListModel *> players;

    QTimer *statesTimer;
};

}  // namespace FastTyping::TextScreen

#endif  // FASTTYPING_TEXTSCREEN_H
