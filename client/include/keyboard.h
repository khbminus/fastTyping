#ifndef FASTTYPING_KEYBOARD_H
#define FASTTYPING_KEYBOARD_H
#include <QColor>
#include <QList>
#include <QObject>
#include <QQmlEngine>
#include <QQmlListProperty>
#include <Qt>
#include <string>

namespace FastTyping::Keyboard {
class KeyboardButtonData : public QObject {
    Q_OBJECT
    Q_PROPERTY(QColor clr READ color CONSTANT)
    Q_PROPERTY(int keycode READ key CONSTANT)
    Q_PROPERTY(int shiftKeycode READ shiftKey CONSTANT)
    Q_PROPERTY(QString letter READ text CONSTANT)
    Q_PROPERTY(QString shiftLetter READ shiftText CONSTANT)
    QML_ELEMENT
    // C++ Class, that provide data to make a layout to the QML widget
    // contains only necessary data
public:
    // some alphabetic key. ShiftKey = key.toUpper()
    KeyboardButtonData(const int &key, const QString &text);
    // key with different symbol at shift, e.g. tick, comma, period, digits
    KeyboardButtonData(const int &key,
                       QString text,
                       const int &shiftKey,
                       QString shiftText);

    void setColor(const QColor &newColor);
    [[nodiscard]] QColor color() const;
    [[nodiscard]] int key() const;
    [[nodiscard]] int shiftKey() const;
    [[nodiscard]] QString text() const;
    [[nodiscard]] QString shiftText() const;

private:
    int mKey;
    int mShiftKey;
    QColor mColor = QColor::fromRgb(53, 50, 47);
    QString mText;
    QString mShiftText;
};

class KeyboardModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<KeyboardButtonData> firstRowModel READ
                   firstRowModel CONSTANT)
    Q_PROPERTY(QQmlListProperty<KeyboardButtonData> secondRowModel READ
                   secondRowModel CONSTANT)
    Q_PROPERTY(QQmlListProperty<KeyboardButtonData> thirdRowModel READ
                   thirdRowModel CONSTANT)
    Q_PROPERTY(QQmlListProperty<KeyboardButtonData> numbersRowModel READ
                   numbersRowModel CONSTANT)
    QML_ELEMENT
public:
    [[nodiscard]] QQmlListProperty<KeyboardButtonData> firstRowModel();
    [[nodiscard]] QQmlListProperty<KeyboardButtonData> secondRowModel();
    [[nodiscard]] QQmlListProperty<KeyboardButtonData> thirdRowModel();
    [[nodiscard]] QQmlListProperty<KeyboardButtonData> numbersRowModel();

    static KeyboardModel *fromFile(const std::string &path);

private:
    QList<QList<KeyboardButtonData *>> mKeyboardRows;
    KeyboardModel() = default;
};

}  // namespace FastTyping::Keyboard

#endif  // FASTTYPING_KEYBOARD_H
