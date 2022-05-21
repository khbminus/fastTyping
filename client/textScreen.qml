import QtQuick 2.0

Item {
    id: veryroot
    height: 51
    width: 701

    property alias model: rootList.model

    Rectangle {
        id: root
        anchors.fill: parent
        border.color: "#000000"
        border.width: 0
        color: "#00ffffff"
        z:1

        ListView {
            id: rootList
            objectName: "textScreen"
            orientation: Qt.Horizontal
            model: root.model
            height: parent.height
            width: parent.width
            anchors.left: parent.left
            anchors.top: parent.top

            z:-1

            delegate: Rectangle {
                    required property bool isCorrect

                    required property bool isCursor
                    required property string letter
                    id: wrapper
                    color: (isCursor ? "#CC747474" : "#00747474")
                    height: parent.height
                    width: charText.width

                    anchors.top: parent.top
                    z:-100
                    Text {
                        anchors.top: parent.top
                        id: charText
                        text: parent.letter
                        color: (parent.isCorrect ? "black" : "darkRed")
                        font.family: "monospace"
                        font.pixelSize: 34
                    }

                    Component.onCompleted: {
                        console.log(isCorrect)
                        console.log(letter)
                    }
                }
            Component.onCompleted: {
                console.log("kek")
                console.log(width, height)
            }
        }
        Component.onCompleted: {
            console.log(width, height)
            console.log(model)
        }
    }
}
