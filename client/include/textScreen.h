#ifndef FASTTYPING_TEXTSCREEN_H
#define FASTTYPING_TEXTSCREEN_H

#include <QAbstractListModel>
#include <QObject>
#include <QQmlEngine>

namespace FastTyping::TextScreen {
Q_NAMESPACE
class ScreenCharPimpl : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isCorrect READ isCorrect CONSTANT)
    Q_PROPERTY(bool isCursor READ isCursorHere CONSTANT)
    Q_PROPERTY(QString letter READ getChar CONSTANT)
public:
    ScreenCharPimpl(QChar c) : letter(c) {}

    [[nodiscard]] QString getChar() const {
        return letter;
    }

    [[nodiscard]] bool isCorrect() const {
        return mIsCorrect;
    }
    [[nodiscard]] bool isCursorHere() const {
        return mIsCursorHere;
    }
    void setCorrectness(bool newValue) {
        mIsCorrect = newValue;
    }
    void setCursor(bool newValue) {
        mIsCursorHere = true;
    }

private:
    QChar letter;
    bool mIsCorrect = true;
    bool mIsCursorHere = false;
};

}  // namespace FastTyping::TextScreen

#endif  // FASTTYPING_TEXTSCREEN_H
