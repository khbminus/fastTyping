#ifndef LOCAL_MANAGER_H
#define LOCAL_MANAGER_H

#include <QObject>
#include <optional>
#include <vector>
#include "WpmChartModel.h"
#include "gameManager.h"
#include "managerUtils.h"

class LocalManager final : public GameManager {
    using Model = FastTyping::Charts::WpmChartModel;
    LocalInputter inputter;
    LocalDictionary dictionary;
    QScopedPointer<Model> wpmChartModel{
        new FastTyping::Charts::WpmChartModel(nullptr)};

    bool check_prefix();
    bool check_symbol(int position);
    void emit_correctness();
    bool is_correct_word();

public:
    explicit LocalManager(std::vector<QString> words);
    void key_pressed(QChar button) override;
    void backspace_pressed() override;
    QString get_buffer() override;
    QVariant next() override;
    QString blob() override;

    Model *getModel() {
        return wpmChartModel.get();
    }
};

class LocalManagerSolo final : public GameManager {
    LocalSoloInputter inputter;
    LocalDictionary dictionary;
    //    bool check_prefix();
    bool check_symbol(int position);
    //    void emit_correctness();
    //    bool is_correct_word();

public:
    explicit LocalManagerSolo(std::vector<QString> words);
    void key_pressed(QChar button) override;
    void backspace_pressed() override;
    QString get_buffer() override;
    QVariant next() override;
    QString blob() override;
};

#endif  // GAME_MANAGER_H
