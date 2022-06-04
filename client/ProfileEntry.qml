import QtQuick 2.0

Item {
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
    id:root
    required property int testsCompleted
    required property date timeTyping
    required property real avgWpm
    required property real maxWpm
    required property real avgAccuracy
    property string favouriteDictionary
    height: firstRow.height + secondRow.height + 10
    Row {
        id: firstRow
        anchors.top: root.top
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 40
        Column {
            spacing: 1
            Text {
                text: "tests completed"
                horizontalAlignment: Text.AlignHLeft
                font.family: loaderSemiBold.name
                font.pointSize: 16
                font.bold: true
            }
            Text {
                text: root.testsCompleted
                horizontalAlignment: Text.AlignHLeft
                font.family: loaderLight.name
                font.pointSize: 12

            }
        }
        Column {
            spacing: 1
            Text {
                text: "time typing"
                horizontalAlignment: Text.AlignHLeft
                font.family: loaderSemiBold.name
                font.pointSize: 16
                font.bold: true
            }
            Text {
                text: root.secondsToString(root.timeTyping.getTime())
                horizontalAlignment: Text.AlignHLeft
                font.family: loaderLight.name
                font.pointSize: 12
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
                font.family: loaderSemiBold.name
                font.pointSize: 16
                font.bold: true
            }
            Text {
                text: root.avgWpm.toFixed(2)
                horizontalAlignment: Text.AlignHLeft
                font.family: loaderLight.name
                font.pointSize: 12
            }
        }
        Column {
            spacing: 1
            Text {
                text: "max wpm"
                horizontalAlignment: Text.AlignHLeft
                font.family: loaderSemiBold.name
                font.pointSize: 16
                font.bold: true
            }
            Text {
                text: root.maxWpm.toFixed(2)
                horizontalAlignment: Text.AlignHLeft
                font.family: loaderLight.name
                font.pointSize: 12
            }
        }
        Column {
            spacing: 1
            Text {
                text: "average accuracy"
                horizontalAlignment: Text.AlignHLeft
                font.family: loaderSemiBold.name
                font.pointSize: 16
                font.bold: true
            }
            Text {
                text: root.avgAccuracy.toFixed(2) + "%"
                horizontalAlignment: Text.AlignHLeft
                font.family: loaderLight.name
                font.pointSize: 12
            }
        }
        Column {
            visible: !!root.favouriteDictionary
            spacing: 1
            Text {
                text: "favourite dictionary"
                horizontalAlignment: Text.AlignHLeft
                font.family: loaderSemiBold.name
                font.pointSize: 16
                font.bold: true
            }
            Text {
                text: root.favouriteDictionary
                horizontalAlignment: Text.AlignHLeft
                font.family: loaderLight.name
                font.pointSize: 12
            }
        }
    }


    function secondsToString(sec) {
        var seconds = sec % 60;
        var minutes = Math.floor(sec / 60) % 60;
        var hours = Math.floor(sec / 3600) % 24;
        var days = Math.floor(sec / 86400);
        var res = ""
        if (days !== 0) {
            res += days + " days "
        }
        if (hours !== 0) {
            res += hours + " hours "
        }
        if (minutes !== 0) {
            res += minutes + " minutes "
        }
        if (seconds !== 0) {
            res += seconds + " seconds "
        }
        return res.trim()
    }
}
