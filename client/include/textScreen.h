#ifndef FASTTYPING_TEXTSCREEN_H
#define FASTTYPING_TEXTSCREEN_H

#include <QAbstractListModel>
#include <QObject>
#include <QQmlEngine>

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
    };
    explicit TextListModel(const QString &words, QObject *parent = nullptr);
    [[nodiscard]] int rowCount(
        const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index,
                                int role = Qt::DisplayRole) const override;

public slots:
    void onWrongChar(int position);
    void onCorrectChar(int position);
    void onMove(const QString &buffer, int position);
    void onNewChar(const QChar &c);
    void onPop();

protected:
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

private:
    void setCorrectnessOfChar(int position, bool value);

    int currentCursor = 0;
    QList<ScreenCharPimpl *> line;
};

}  // namespace FastTyping::TextScreen

#endif  // FASTTYPING_TEXTSCREEN_H
