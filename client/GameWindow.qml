import QtQuick 2.0
import QtQuick.Controls 2.15

Item {
    id: root
    width: 800
    height: 600
    property int gameId: 0
    property var keyModel
    property var textModel

    focus: true
    Keys.onPressed: {
        pressedKey(event.key, event.text)
    }
    Keys.onReleased: {
        releasedKey(event.key, event.text)
    }

    signal pressedKey(key: int, text: string)
    signal releasedKey(key: int, text: string)
    signal returnPressed()

    Text {
        id: gameIDview
        x: 14
        y: 550

        text: "Game ID: " + parent.gameId
    }
    Keyboard {
        id: keyboard

        x: 50
        y: 280
        height: 261
        width: 701
        keyModel: parent.keyModel
    }
    TextScreen {
        id: textScreen

        x: 50
        y: 30
        height: 201
        width: 701
        model: parent.textModel
    }
    Button {
        x: 700
        y: 570
        height: 32
        width: 100
        id: returnButton
        text: "return"
        onClicked: root.returnPressed()
    }

    function moveCursor1(position) {
        textScreen.moveCursor1(position)
    }
    function pressKey(key) {
        keyboard.pressKey(key)
    }

    function releaseKey(key) {
        keyboard.releaseKey(key)
    }

    function shiftHighlight() {
        keyboard.shiftHighlight()
    }

    function backspaceHighlight() {
        keyboard.backspaceHighlight()
    }

    function clearHighlight() {
        keyboard.clearHighlight()
    }

}
