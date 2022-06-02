import QtQuick 2.0
import QtQuick.Controls 2.15

Item {
    id: root
    width: 800
    height: 600
    signal returnPressed()

    Text {
        id: userName
        text: "Aboba"
        anchors.top: parent.top
        anchors.topMargin: 15
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: 24
    }

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
    Row {
        id: firstRow
        anchors.top: userName.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 40
        Column {
            spacing: 1
            Text {
                text: "tests completed"
                horizontalAlignment: Text.AlignHLeft
            }
            Text {
                text: "1337"
                horizontalAlignment: Text.AlignHLeft
            }
        }
        Column {
            spacing: 1
            Text {
                text: "I dunno? time typing?"
                horizontalAlignment: Text.AlignHLeft
            }
            Text {
                text: "-e^π"
                horizontalAlignment: Text.AlignHLeft
            }
        }
    }
    Row {
        id: secondRow
        anchors.top: firstRow.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 40
        Column {
            spacing: 1
            Text {
                text: "avg wpm"
                horizontalAlignment: Text.AlignHLeft
            }
            Text {
                text: "69"
                horizontalAlignment: Text.AlignHLeft
            }
        }
        Column {
            spacing: 1
            Text {
                text: "max wpm"
                horizontalAlignment: Text.AlignHLeft
            }
            Text {
                text: "88"
                horizontalAlignment: Text.AlignHLeft
            }
        }
        Column {
            spacing: 1
            Text {
                text: "average accuracy"
                horizontalAlignment: Text.AlignHLeft
            }
            Text {
                text: "10%"
                horizontalAlignment: Text.AlignHLeft
            }
        }
        Column {
            spacing: 1
            Text {
                text: "favourite dictionary"
                horizontalAlignment: Text.AlignHLeft
            }
            Text {
                text: "Алгебра М.А. Антипов"
                horizontalAlignment: Text.AlignHLeft
            }
        }
    }
}
