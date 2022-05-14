#include "keyboard.h"
#include <QtDebug>
#include <QtGlobal>
#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>

// namespace FastTyping::Client {
KeyboardButtonData::KeyboardButtonData(const int &key, const QString &text)
    : mKey(key), mShiftKey(key), mText(text), mShiftText(text.toUpper()) {}
KeyboardButtonData::KeyboardButtonData(const int &key,
                                       QString text,
                                       const int &shiftKey,
                                       QString shiftText)
    : mKey(key),
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

KeyboardModel *KeyboardModel::fromFile(const std::string &path) {
    std::ifstream layoutFile(path, std::ios::in);
    nlohmann::json arr;
    auto res = new KeyboardModel();

    if (!(layoutFile >> arr)) {
        qFatal("Error with opening/reading file %s!", path.c_str());
    }
    if (!arr.is_array()) {
        qFatal("Invalid layout format: json is not an array");
    }
    for (const auto &rowJson : arr) {
        QList<KeyboardButtonData *> rowList;
        if (!rowJson.is_array()) {
            qFatal("Invalid layout format: json is not an array of arrays");
        }
        for (const auto &keyJson : rowJson) {
            if (!keyJson.contains("key") || !keyJson.contains("text")) {
                qFatal("Invalid layout format: can't find text/key");
            }
            QString text =
                QString::fromStdString(keyJson["text"].get<std::string>());
            int key = keyJson["key"].get<int>();
            KeyboardButtonData *btn;
            if (keyJson.contains("shiftKey") || keyJson.contains("shiftText")) {
                if (!keyJson.contains("shiftKey") ||
                    !keyJson.contains("shiftText")) {
                    qFatal(
                        "Invalid layout format: can't find shiftKey/shiftText, "
                        "but another one is found");
                }
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
        res->mKeyboardRows << rowList;
        qDebug() << "loaded a row!";
    }
    return res;
}

//}  // namespace FastTyping::Client