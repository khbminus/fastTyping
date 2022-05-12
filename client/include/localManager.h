#ifndef LOCAL_MANAGER_H
#define LOCAL_MANAGER_H

#include "gameManager.h"
#include "managerUtils.h"

#include <QObject>
#include <optional>
#include <vector>

class LocalManager final : public GameManager {
    LocalInputter inputter;
    LocalDictionary dictionary;
    bool check_prefix();
    void emit_correctness();

public:
    LocalManager(std::vector<QString> words);
    void key_pressed(QChar button) override;
    void backspace_pressed() override;
    QString get_buffer() override;
    std::optional<QChar> next() override;
    QString blob() override;
};

#endif// GAME_MANAGER_H
