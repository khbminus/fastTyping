import QtQuick 2.0

Item {
    id: veryroot
    height: 102
    width: 701

    property alias model: rootList.model
    property alias model2: rootList2.model

    Rectangle {
        id: root
        border.color: "#000000"
        border.width: 0
        height: parent.height / 2
        width: parent.width
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
                color: (isCursor ? "#CC747474" : ((letter === " " && !isCorrect) ? "#CC8B0000" : "#00747474"))
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
            }
        }
    }
    Rectangle {
        id: root2
        anchors.top: root.bottom
        height: parent.height / 2
        width: parent.width
        border.color: "#000000"
        border.width: 0
        color: "#00ffffff"
        z:1

        ListView {
            id: rootList2
            objectName: "textScreen"
            orientation: Qt.Horizontal
            model: root.model
            height: parent.height
            width: parent.width
            anchors.left: parent.left
            anchors.top: parent.top

            z:-1

            delegate: Rectangle {
                required property string letter
                id: wrapper2
                color: "#00747474"
                height: parent.height
                width: charText2.width

                anchors.top: parent.top
                z:-100
                Text {
                    anchors.top: parent.top
                    id: charText2
                    text: parent.letter
                    color: "black"
                    font.family: "monospace"
                    font.pixelSize: 34
                }
            }
        }
    }
}
