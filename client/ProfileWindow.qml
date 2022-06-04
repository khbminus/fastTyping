import QtQuick 2.3
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

Item {
    id: root
    width: 800
    height: 600

    required property var mainInfo
    required property var dictsNames
    required property var dictsValues
    required property string userName

    signal returnPressed()

    FontLoader {
        id: loaderLight
        source: "https://github.com/JetBrains/JetBrainsMono/blob/master/fonts/ttf/JetBrainsMono-Light.ttf?raw=true"
    }
    FontLoader {
        id: loaderSemiBold
        source: "https://github.com/JetBrains/JetBrainsMono/blob/master/fonts/ttf/JetBrainsMono-SemiBold.ttf?raw=true"
    }
    FontLoader {
        id: loaderBold
        source: "https://github.com/JetBrains/JetBrainsMono/blob/master/fonts/ttf/JetBrainsMono-Bold.ttf?raw=true"
    }

    Text {
        id: userName
        text: "User name"
        anchors.top: parent.top
        anchors.topMargin: 15
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: 24
    }
    Text {
        id: commonInfoLabel
        text: "General Stats:"
        anchors.left: root.left
        anchors.top: userName.bottom
        font.family: loaderBold.name
        font.pointSize: 18
        font.bold: true
        font.underline: true
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

    ProfileEntry {
        id: mainEntry
        anchors.top: commonInfoLabel.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        testsCompleted: mainInfo.testsCompleted
        timeTyping: mainInfo.timeTyping
        avgAccuracy: mainInfo.avgAccuracy
        avgWpm: mainInfo.avgWpm
        maxWpm: mainInfo.maxWpm
        favouriteDictionary: mainInfo.favouriteDictionary
    }

    Text {
        id: perDictLabel
        text: "Per Dictionary Stats:"
        anchors.left: root.left
        anchors.top: mainEntry.bottom
        font.family: loaderBold.name
        font.pointSize: 18
        font.bold: true
        font.underline: true
    }

    TabBar {
        id: dictsBar
        anchors.left: root.left
        anchors.top: perDictLabel.bottom
        anchors.topMargin: 5
        width: root.width

        Repeater {
            model: root.dictsNames

            TabButton {
                text: modelData
                font.family: loaderSemiBold.name
                width: Math.max(100, dictsBar.width / 4)
            }
        }
    }
    StackLayout {
        id: dictsLayout
        anchors.left: root.left
        anchors.right: root.right
        anchors.top: dictsBar.bottom
        width: root.width
        height: mainEntry.height // tricky hacks
        currentIndex: dictsBar.currentIndex
        Repeater {
            model: root.dictsValues
            delegate: ProfileEntry {
                required testsCompleted
                required timeTyping
                required avgAccuracy
                required avgWpm
                required maxWpm
            }
        }
    }

    Text {
        id: allGamesLabel
        text: "Recent Games:"
        anchors.left: root.left
        anchors.top: dictsLayout.bottom
        font.family: loaderBold.name
        font.pointSize: 18
        font.bold: true
        font.underline: true
    }
}
