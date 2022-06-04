#ifndef WEB_MANAGER_H
#define WEB_MANAGER_H

#include <QList>
#include <QObject>
#include <QVector>
#include <optional>
#include <vector>
#include "gameManager.h"
#include "managerUtils.h"

class WebManager : public GameManager {
    LocalInputter inputter;

protected:
    LocalDictionary dictionary;

public:
    explicit WebManager(std::vector<QString> words);
    void key_pressed(QChar button) override;
    void backspace_pressed() override;
    QString get_buffer() override;
    QVariant next() override;
    QString blob() override;

public slots:
    void end_slot();
    void error_slot();
    void correct_slot();
    virtual void correct_word_slot();
};

class WebSoloManager final : public WebManager {
    LocalSoloInputter inputter;

public:
    explicit WebSoloManager(std::vector<QString> words);
    void key_pressed(QChar button) override;
    void backspace_pressed() override;
    QString get_buffer() override;
    QVariant next() override;
    QString blob() override;

public slots:
    void end_slot();
    void error_slot();
    void correct_slot();
    void correct_word_slot() override;
};

#endif  // GAME_MANAGER_H
