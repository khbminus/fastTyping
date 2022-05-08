#ifndef WEB_MANAGER_H
#define WEB_MANAGER_H

#include "gameManager.h"
#include "managerUtils.h"

#include <QObject>
#include <QList>
#include <QVector>
#include <optional>
#include <vector>

class WebManager final : public GameManager {
    LocalInputter inputter;
    LocalDictionary dictionary;
public:
    WebManager(std::vector<QString> words);
    void key_pressed(QChar button) override;
    void backspace_pressed() override;
    QString get_buffer() override;
    std::optional<QChar> next() override;
    QString blob() override;
public slots:
    void end_slot();
    void error_slot();
    void correct_slot();
    void correct_word_slot();
};

#endif// GAME_MANAGER_H
