#ifndef FASTTYPING_KEYBOARD_H
#define FASTTYPING_KEYBOARD_H
#include <QAbstractListModel>
#include <QColor>
#include <QList>
#include <QObject>
#include <QQmlEngine>
#include <QQmlListProperty>
#include <Qt>
#include <nlohmann/json.hpp>
#include <string>

namespace FastTyping::Keyboard {
Q_NAMESPACE
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
    KeyboardButtonData(const int &key,
                       const QString &text,
                       QObject *parent = nullptr);
    // key with different symbol at shift, e.g. tick, comma, period, digits
    KeyboardButtonData(const int &key,
                       QString text,
                       const int &shiftKey,
                       QString shiftText,
                       QObject *parent = nullptr);

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
                   firstRowModel NOTIFY rowsChanged)
    Q_PROPERTY(QQmlListProperty<KeyboardButtonData> secondRowModel READ
                   secondRowModel NOTIFY rowsChanged)
    Q_PROPERTY(QQmlListProperty<KeyboardButtonData> thirdRowModel READ
                   thirdRowModel NOTIFY rowsChanged)
    Q_PROPERTY(QQmlListProperty<KeyboardButtonData> numbersRowModel READ
                   numbersRowModel NOTIFY rowsChanged)
    QML_ELEMENT
public:
    [[nodiscard]] QQmlListProperty<KeyboardButtonData> firstRowModel();
    [[nodiscard]] QQmlListProperty<KeyboardButtonData> secondRowModel();
    [[nodiscard]] QQmlListProperty<KeyboardButtonData> thirdRowModel();
    [[nodiscard]] QQmlListProperty<KeyboardButtonData> numbersRowModel();

    static KeyboardModel &getInstance() {
        static KeyboardModel model;
        return model;
    }

    void addPath(const QString &path);
    void removePath(const QString &path);
    void setCurrentLayout(qsizetype idx);
    [[nodiscard]] qsizetype getCurrentLayout() const;
signals:
    void rowsChanged();

private:
    struct LayoutDescription {
        LayoutDescription(QString path, QString name, QString description);
        QString path;
        QString name;
        QString description;
    };

    //    class LayoutItemModel : public QAbstractItemModel {
    //        Q_OBJECT
    //    public:
    //        explicit LayoutItemModel(QObject *parent = nullptr);
    //
    //        QVariant data(const QModelIndex& index, int role =
    //        Qt::DisplayRole) const override; Qt::ItemFlags flags(const
    //        QModelIndex& index) const override;
    //
    //    };

    qsizetype currentLayout;
    QList<QList<KeyboardButtonData *>> mKeyboardRows;

    KeyboardModel() : QObject(nullptr) {}
    void loadFromFile(const QString &path);
    void validate(const nlohmann::json &layout);
    // TODO: Change with SQL table
    QList<LayoutDescription> layouts;
};

}  // namespace FastTyping::Keyboard

#endif  // FASTTYPING_KEYBOARD_H
