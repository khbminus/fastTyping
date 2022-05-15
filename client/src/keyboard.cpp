#include "keyboard.h"
#include <QFile>
#include <QtDebug>
#include <QtGlobal>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <utility>

namespace FastTyping::Keyboard {
KeyboardButtonData::KeyboardButtonData(const int &key,
                                       const QString &text,
                                       QObject *parent)
    : QObject(parent),
      mKey(key),
      mShiftKey(key),
      mText(text),
      mShiftText(text.toUpper()) {}
KeyboardButtonData::KeyboardButtonData(const int &key,
                                       QString text,
                                       const int &shiftKey,
                                       QString shiftText,
                                       QObject *parent)
    : QObject(parent),
      mKey(key),
      mShiftKey(shiftKey),
      mText(std::move(text)),
      mShiftText(std::move(shiftText)) {}

QString KeyboardButtonData::shiftText() const {
    return mShiftText;
}
QString KeyboardButtonData::text() const {
    return mText;
}
int KeyboardButtonData::shiftKey() const {
    return mShiftKey;
}
int KeyboardButtonData::key() const {
    return mKey;
}
QColor KeyboardButtonData::color() const {
    return mColor;
}

void KeyboardButtonData::setColor(const QColor &newColor) {
    mColor = newColor;
}
QQmlListProperty<KeyboardButtonData> KeyboardModel::firstRowModel() {
    return {this, &mKeyboardRows[1]};
}
QQmlListProperty<KeyboardButtonData> KeyboardModel::secondRowModel() {
    return {this, &mKeyboardRows[2]};
}
QQmlListProperty<KeyboardButtonData> KeyboardModel::thirdRowModel() {
    return {this, &mKeyboardRows[3]};
}
QQmlListProperty<KeyboardButtonData> KeyboardModel::numbersRowModel() {
    return {this, &mKeyboardRows[0]};
}

void KeyboardModel::loadFromFile(const QString &path) {
    QFile layoutFile(path);
    if (!layoutFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qFatal("Error with opening file!");
    }
    auto read = QString(layoutFile.readAll());
    layoutFile.close();

    nlohmann::json arr = nlohmann::json::parse(read.toStdWString());

    validate(arr);
    mKeyboardRows.clear();
    for (const auto &rowJson : arr) {
        QList<KeyboardButtonData *> rowList;
        for (const auto &keyJson : rowJson) {
            QString text =
                QString::fromStdString(keyJson["text"].get<std::string>());
            int key = keyJson["key"].get<int>();
            KeyboardButtonData *btn = nullptr;
            if (keyJson.contains("shiftKey") || keyJson.contains("shiftText")) {
                QString shiftText = QString::fromStdString(
                    keyJson["shiftText"].get<std::string>());
                int shiftKey = keyJson["shiftKey"].get<int>();
                btn = new KeyboardButtonData(key, text, shiftKey, shiftText);
                qDebug() << "loading a shift-contain key: " << btn->text()
                         << " " << btn->key() << " " << btn->shiftText() << " "
                         << btn->shiftKey();
            } else {
                btn = new KeyboardButtonData(key, text);
                qDebug() << "loading a non shift-contain key: " << btn->text()
                         << " " << btn->key() << " " << btn->shiftText() << " "
                         << btn->shiftKey();
            }

            if (keyJson.contains("color")) {
                btn->setColor(QColor(QString::fromStdString(
                    keyJson["color"].get<std::string>())));
            }  // TODO: add default color
            rowList << btn;
        }
        mKeyboardRows << rowList;
        qDebug() << "loaded a row!";
    }
}

void KeyboardModel::addPath(const QString &path) {
    QFile layoutFile(path);
    if (!layoutFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qFatal("Error with opening file!");
    }
    auto read = QString(layoutFile.readAll());
    layoutFile.close();

    nlohmann::json arr = nlohmann::json::parse(read.toStdWString());
    validate(arr);  // TODO: exception are really needed

    layouts.emplace_back(path, path.split('/').last(), QString());
    // FIXME: rly? This is disgusting
}

void KeyboardModel::removePath(const QString &path) {
    layouts.removeIf(
        [&](const LayoutDescription &l) { return l.path == path; });
}

void KeyboardModel::setCurrentLayout(qsizetype idx) {
    loadFromFile(layouts[idx].path);
    emit rowsChanged();
}

void KeyboardModel::validate(const nlohmann::json &layout) {
    if (!layout.is_array()) {
        qFatal("Invalid layout format: json is not an array");
    }

    if (layout.size() != 4) {
        qFatal("Invalid layout format: rows count be 4");
    }

    std::size_t currentKeys = 13;

    for (const auto &rowJson : layout) {
        if (!rowJson.is_array()) {
            qFatal("Invalid layout format: json is not an array of arrays");
        }
        if (rowJson.size() != currentKeys) {
            qFatal("Invalid layout format: row has size %lu, but should %lu",
                   rowJson.size(), currentKeys);
        }
        currentKeys--;
        for (const auto &keyJson : rowJson) {
            if (!keyJson.contains("key") || !keyJson.contains("text")) {
                qFatal("Invalid layout format: can't find text/key");
            }
            if (keyJson.contains("shiftKey") || keyJson.contains("shiftText")) {
                if (!keyJson.contains("shiftKey") ||
                    !keyJson.contains("shiftText")) {
                    qFatal(
                        "Invalid layout format: can't find shiftKey/shiftText, "
                        "but another one is found");
                }
            }
        }
    }
}
qsizetype KeyboardModel::getCurrentLayout() const {
    return currentLayout;
}

KeyboardModel::LayoutDescription::LayoutDescription(QString path,
                                                    QString name,
                                                    QString description)
    : path(std::move(path)),
      name(std::move(name)),
      description(std::move(description)) {}
}  // namespace FastTyping::Keyboard