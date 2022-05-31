import QtQuick 2.0


Rectangle {
    id: root
    border.color: "black"
    border.width: 1
    opacity: 1
    clip: true
    radius: 6
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
        clip: true

        //z:-1

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
                color: (parent.isCursor ? "#CC747474" : ((!parent.isCorrect) ? "#CC8B0000" : "#00747474"))
                height: 51
                width: charText.width

                Text {
                    FontLoader {
                        id: loader
                        source: "https://dtinth.github.io/comic-mono-font/ComicMono.ttf"
                    }
                    anchors.top: parent.top
                    id: charText
                    text: row.letter
                    color: (row.isCorrect ? "black" : "white")
                    font.family: loader.name
                    font.pixelSize: 34
                }
            }
            ListView {
                id: playerList
                anchors.left: wrapper.horizontalCenter
                anchors.leftMargin: -4
                width: parent.width
                height: parent.height
                model: parent.players
                delegate: Item {
                    required property color color
                    required opacity
                    required property string username
                    width: playerRect.width + 2 + playerName.width
                    height: playerName.height
                    Rectangle {
                        id: playerRect
                        width: 8
                        height: 8
                        color:parent.color
                    }
                    Text {
                        anchors.left: playerRect.right
                        anchors.leftMargin: 2
                        anchors.verticalCenter: playerRect.verticalCenter

                        id: playerName
                        text: "— " + parent.username
                    }
                }
            }
        }
    }
}

