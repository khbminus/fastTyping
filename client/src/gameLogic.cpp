#include "gameLogic.h"
#include <QString>
#include <iostream>
//#include "gamewindow.h"

namespace game {


    void RaceGame::checkPrefix(QString a, QString b) {
        if (a.size() > b.size()) {
            errorState = true;
            return;
        }
        for (int ind = 0; ind < a.size(); ind++) {
            if (a[ind] != b[ind]) {
                errorState = true;
                return;
            }
        }
        errorState = false;
    }

    RaceGame::RaceGame() : dictionary({"This", "is", "sample", "don't", "judge", "me"}), errorState(false), endOfGame(false){};

    void RaceGame::keyPressed(QChar button) {
        if (button == ' ') {
            if (inputter.getBuffer() == dictionary.getCurrentWord()) {
                inputter.clearBuffer();
                if (!dictionary.nextWord()) {
                    errorState = false;
                    endOfGame = true;
                    return;
                }
            } else {
                inputter.addSymbol(button);
            }
        } else {
            inputter.addSymbol(button);
        }
        checkPrefix(inputter.getBuffer(), dictionary.getCurrentWord());
    }
    void RaceGame::backspacePressed() {
        inputter.deleteSymbol();
        checkPrefix(inputter.getBuffer(), dictionary.getCurrentWord());
    }

    [[nodiscard]] bool RaceGame::getErrorStatus() const {
        return errorState;
    }

    [[nodiscard]] bool RaceGame::isEnded() const {
        return endOfGame;
    }
    
    [[nodiscard]] QString RaceGame::getBuffer() const {
        return inputter.getBuffer();
    }

}// namespace game
