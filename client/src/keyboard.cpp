#include "keyboard.h"
#include <QBrush>
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
    mIsUpperMap.clear();
    for (const auto &rowJson : arr["keys"]) {
        QList<KeyboardButtonData *> rowList;
        for (const auto &keyJson : rowJson) {
            QString text =
                QString::fromStdString(keyJson["text"].get<std::string>());

            if (text.size() != 1) {
                qWarning(
                    "text size is %lld, but should be one (it can be unicode "
                    "trouble, yea). Text: %s",
                    text.size(), text.toLocal8Bit().data());
            }
            mIsUpperMap[text[0]] = false;
            int key = keyJson["key"].get<int>();
            KeyboardButtonData *btn = nullptr;
            if (keyJson.contains("shiftKey") || keyJson.contains("shiftText")) {
                QString shiftText = QString::fromStdString(
                    keyJson["shiftText"].get<std::string>());
                if (shiftText.size() != 1) {
                    qWarning(
                        "shiftText size is %lld, but should be one (it can be "
                        "unicode trouble, yea). ShiftText: %s",
                        shiftText.size(), shiftText.toLocal8Bit().data());
                }
                int shiftKey = keyJson["shiftKey"].get<int>();
                btn = new KeyboardButtonData(key, text, shiftKey, shiftText,
                                             this);
                mIsUpperMap[shiftText[0]] = true;

                qDebug() << "loading a shift-contain key: " << btn->text()
                         << " " << btn->key() << " " << btn->shiftText() << " "
                         << btn->shiftKey();
            } else {
                btn = new KeyboardButtonData(key, text, this);
                qDebug() << "loading a non shift-contain key: " << btn->text()
                         << " " << btn->key() << " " << btn->shiftText() << " "
                         << btn->shiftKey();
                mIsUpperMap[text.toUpper()[0]] = true;
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

    layouts.push_back(
        LayoutDescription(path, QString::fromStdString(arr["name"]),
                          QString::fromStdString(arr["description"])));
}

void KeyboardModel::removePath(const QString &path) {
    layouts.removeIf(
        [&](const LayoutDescription &l) { return l.path == path; });
}

void KeyboardModel::setCurrentLayout(qsizetype idx) {
    loadFromFile(layouts[idx].path);
    currentLayout = idx;
    emit rowsChanged();
}

void KeyboardModel::validate(const nlohmann::json &layout) {
    if (!layout.contains("keys") || !layout.contains("name") ||
        !layout.contains("description")) {
        qFatal("Invalid layout format: bad format");
    }

    if (!layout["name"].is_string()) {
        qFatal("Invalid layout format: name must be string");
    }
    if (!layout["description"].is_string()) {
        qFatal("Invalid layout format: description must be string");
    }

    if (!layout["keys"].is_array()) {
        qFatal("Invalid layout format: keys is not an array");
    }

    if (layout["keys"].size() != 4) {
        qFatal("Invalid layout format: rows number be 4");
    }

    std::size_t currentKeys = 13;

    for (const auto &rowJson : layout["keys"]) {
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

LayoutDescription::LayoutDescription(QString path,
                                     QString name,
                                     QString description)
    : path(std::move(path)),
      name(std::move(name)),
      description(std::move(description)) {}
int LayoutTableModel::rowCount(const QModelIndex &parent) const {
    return model->layouts.size();
}
int LayoutTableModel::columnCount(const QModelIndex &) const {
    return 3;
}
QVariant LayoutTableModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0:
                return model->layouts[index.row()].name;
            case 1:
                return model->layouts[index.row()].description;
            case 2:
                return model->layouts[index.row()].path;
        }
    }
    if (role == Qt::BackgroundRole && index.row() == model->currentLayout) {
        auto res = QBrush(Qt::SolidPattern);
        res.setColor(QColor::fromRgba64(QRgba64::fromRgba(48, 197, 255, 30)));
        return res;
    }
    return {};
}

QAbstractTableModel *KeyboardModel::tableModel() {
    auto *model = new LayoutTableModel(this, this);
    return model;
}
LayoutTableModel::LayoutTableModel(KeyboardModel *model, QObject *parent)
    : QAbstractTableModel(parent), model(model) {
    connect(model, &KeyboardModel::rowsChanged, [&]() {
        qDebug() << "signalled redraw!";
        emit dataChanged(index(0, 0), index(rowCount() - 1, 2));
    });
}
QVariant LayoutTableModel::headerData(int section,
                                      Qt::Orientation orientation,
                                      int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return QString("Name");
            case 1:
                return QString("Description");
            case 2:
                return QString("Path");
        }
    }
    return {};
}

bool KeyboardModel::isUpper(QChar c) {
    return mIsUpperMap[c];
}

}  // namespace FastTyping::Keyboard