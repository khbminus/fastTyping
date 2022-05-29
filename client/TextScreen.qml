import QtQuick 2.0


Rectangle {
    id: root
    border.color: "black"
    border.width: 1
    opacity: 1
    clip: true
    property alias model : rootList.model
    function moveCursor1(position) {
            if (position >= 1) {
                position--
            }
            console.log("move at position ", position)
            rootList.positionViewAtIndex(position, ListView.Center)
    }

    ListView {
        id: rootList
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        objectName: "textScreen"
        orientation: Qt.Horizontal
        flickableDirection: Flickable.AutoFlickDirection

        //z:-1

        delegate: Rectangle {
            clip: true
            required property bool isCorrect

            required property bool isCursor
            required property string letter
            id: wrapper
            color: (isCursor ? "#CC747474" : ((!isCorrect) ? "#CC8B0000" : "#00747474"))
            height: parent.height
            width: charText.width

            anchors.top: parent.top
            //z:-100
            Text {
                anchors.top: parent.top
                id: charText
                text: parent.letter

                color: (parent.isCorrect ? "black" : "white")
                font.family: "monospace"
                font.pixelSize: 34
            }
        }
    }
}

