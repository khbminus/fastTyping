import QtQuick 2.3
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

Item {
    id: root
    width: 800
    height: 600

    /*required property var mainInfo
    required property var dictsNames
    required property var dictsValues
    required property string userName*/

    signal returnPressed()

    FontLoader {
        id: loaderLight
        source: "https://github.com/JetBrains/JetBrainsMono/blob/master/fonts/ttf/JetBrainsMono-Light.ttf?raw=true"
    }
    FontLoader {
        id: loaderSemiBold
        source: "https://github.com/JetBrains/JetBrainsMono/blob/master/fonts/ttf/JetBrainsMono-SemiBold.ttf?raw=true"
    }

    Text {
        id: userName
        text: "User name"
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

    ProfileEntry {
        id: mainEntry
        anchors.top: userName.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        testsCompleted: 1337
        timeTyping: new Date(304643)
        avgAccuracy: 33.33333
        avgWpm: 167
        maxWpm: 239
        favouriteDictionary: "Абоба-2"

    }

    TabBar {
        id: dictsBar
        anchors.left: root.left
        anchors.top: mainEntry.bottom
        anchors.topMargin: 5
        width: root.width

        Repeater {
            model: ["English", "Russian", "Алгебра М.А. Антипов", "Баги Юрия Зайцева", "Охуенные истории Никиты Андреева", "Список предателей родины"]

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
        currentIndex: dictsBar.currentIndex
        ProfileEntry {
            testsCompleted: 1
            timeTyping: new Date(10801)
            avgAccuracy: 69.69
            avgWpm: 120
            maxWpm: 566
        }
        ProfileEntry {
            testsCompleted: 2
            timeTyping: new Date(10801)
            avgAccuracy: 69.69
            avgWpm: 120
            maxWpm: 566
        }
        ProfileEntry {
            testsCompleted: 3
            timeTyping: new Date(10801)
            avgAccuracy: 69.69
            avgWpm: 120
            maxWpm: 566
        }
        ProfileEntry {
            testsCompleted: 4
            timeTyping: new Date(10801)
            avgAccuracy: 69.69
            avgWpm: 120
            maxWpm: 566
        }
        ProfileEntry {
            testsCompleted: 5
            timeTyping: new Date(10801)
            avgAccuracy: 69.69
            avgWpm: 120
            maxWpm: 566
        }
        ProfileEntry {
            testsCompleted: 6
            timeTyping: new Date(10801)
            avgAccuracy: 69.69
            avgWpm: 120
            maxWpm: 566
        }
    }
}
