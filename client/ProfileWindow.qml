import QtQuick 2.0
import QtQuick.Controls 2.15

Item {
    id: root
    width: 800
    height: 600
    signal returnPressed()

    Button {
        x: 700
        y: 570
        height: 32
        width: 100
        id: returnButton
        text: "return"
        onClicked: root.returnPressed()
        focusPolicy: Qt.ClickFocus
    }
}
