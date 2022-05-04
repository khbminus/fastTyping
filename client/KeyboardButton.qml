import QtQuick 2.0

Item {
    id:root
    property color color: "#35322f"
    property string text
    property int key
    property var inputPanel
    property alias displayText: txt.text
    property bool isHighlighted: false
    property bool isNext: false

    Connections {
        target: inputPanel
        function onPressed(keyPressed) {
            if (root.key === keyPressed) {
                console.log(text, "Pressed")
                root.isHighlighted = true
            }
        }
        function onReleased(keyReleased) {
            if (root.key === keyReleased) {
                console.log(text, "Released")
                root.isHighlighted = false
            }
        }
    }

        Rectangle {
                anchors.fill: parent
                border.color: "#000000"
                border.width: isNext ? 3 : 0
                //anchors.horizontalCenter: parent.horizontalCenter
                //anchors.verticalCenter: parent.verticalCenter
                //anchors.margins: 0.1
                radius: height / 30
                color: isHighlighted ? Qt.tint(root.color, "#801e6fa7") : root.color
                Text {
                    id: txt
                    color: "black"
                    anchors.margins: parent.height / 6
                    anchors.fill: parent
                    fontSizeMode: Text.Fit
                    font.pixelSize: height
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: root.text
                }
    }
}
