import QtQuick 2.0

ListView {
    id: root
    objectName: "textScreen"
    orientation: Qt.Horizontal
    required model
    height: 100
    width: 500

    delegate: Rectangle {
            required property bool isCorrect

            required property bool isCursor
            required property string letter
            id: wrapper
            width: charText.width
            color: (isCursor ? "#CC747474" : "#00747474")
            height: charText.height

            Text {
                id: charText
                text: parent.letter
                color: (parent.isCorrect ? "black" : "darkRed")
                font.family: "JetBrainsMono Nerd Font"
                font.pointSize: 48
            }

            Component.onCompleted: {
                console.log(isCorrect)
                console.log(letter)
            }
        }
    Component.onCompleted: {
        console.log(itemAtIndex(0))
        console.log("kek")
        console.log(model)
    }
}
