import QtQuick 2.0


ListView {
    id: rootList

    height: 102
    width: 701
    objectName: "textScreen"
    orientation: Qt.Horizontal
    anchors.left: parent.left
    anchors.top: parent.top
    flickableDirection: Flickable.AutoFlickDirection

    z:-1
    function moveCursor1(position) {
            if (position >= 1) {
                position--
            }
            console.log("move at position ", position)
            positionViewAtIndex(position, ListView.Center)
        }

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

