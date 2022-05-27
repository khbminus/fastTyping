import QtQuick 2.0


ListView {
    id: rootList

    height: 102
    width: 701
    objectName: "textScreen"
    orientation: Qt.Horizontal
    flickableDirection: Flickable.AutoFlickDirection
    interactive: false

    z:-1
    function moveCursor1(position) {
            if (position >= 1) {
                position--
            }
            console.log("move at position ", position)
            positionViewAtIndex(position, ListView.Center)
        }

    delegate: Column {
        required property bool isCorrect
        required property bool isCursor
        required property string letter
        required property var players
        id: row
        spacing: 2
        height: parent.height
        width: charText.width
        Rectangle {
            id: wrapper
            color: (row.isCursor ? "#CC747474" : ((row.letter === " " && !row.isCorrect) ? "#CC8B0000" : "#00747474"))
            height: 51
            width: charText.width

            z:-100
            Text {
                anchors.top: parent.top
                id: charText
                text: row.letter
                color: (row.isCorrect ? "black" : "darkRed")
                font.family: "monospace"
                font.pixelSize: 34
            }
        }

        ListView {
            height: parent.height - wrapper.height
            id: playersList
            width:parent.width
            model: parent.players
            delegate: Rectangle {
                required property string modelData
                anchors.horizontalCenter: parent.horizontalCenter
                //anchors.fill: parent
                id: playerRect
                height: 8
                width: 8
                color: modelData
            }
        }
    }
}

