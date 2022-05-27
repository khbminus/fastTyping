#ifndef FASTTYPING_TEXTSCREEN_H
#define FASTTYPING_TEXTSCREEN_H

#include <QAbstractListModel>
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
    QTimer *statesTimer;
};

}  // namespace FastTyping::TextScreen

#endif  // FASTTYPING_TEXTSCREEN_H
