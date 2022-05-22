import QtQuick 2.0

Item {
    id: veryroot
    height: 102
    width: 701

    property alias model: rootList.model
    function moveCursor1(position) {
        if (position >= 1) {
            position--
        }
        console.log("move at position ", position)
        rootList.positionViewAtIndex(position, ListView.Center)
    }

    Rectangle {
        id: root
        border.color: "#000000"
        border.width: 0
        height: parent.height
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
            flickableDirection: Flickable.AutoFlickDirection

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
}
