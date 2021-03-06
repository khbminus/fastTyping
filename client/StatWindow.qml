import QtQuick 2.0
import QtQuick.Controls 2.15
import QtCharts 2.15

Item {
    id: root
    width: 800
    height: 600
    property int gameId: 0
    property var textModel
    property string rawWPM
    property string onlyCorrectWPM
    property int charsTyped
    property int charsCorrect
    property var chartModel
    property int maxChartWPM
    property int maxChartErrors

    signal returnPressed()

    Text {
        id: gameIDview
        x: 14
        y: 550

        text: "Game ID: " + parent.gameId
    }
    TextScreen {
        id: textScreen

        x: 50
        y: 30
        height: 201
        width: 701
        model: parent.textModel
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
        id: statsRow
        anchors.left: textScreen.left
        anchors.top: textScreen.bottom
        anchors.topMargin: 6
        spacing: 6

        Rectangle {
            border.width: 1
            radius: 6
            height: 48
            width: 100
            Column {
                anchors.fill: parent
                Text {
                    id: rawWPMlabel
                    width: parent.width
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 20
                    font.bold: true
                    text: "RAW WPM"
                }
                Text {
                    id:rawWPMnumber
                    width: parent.width
                    horizontalAlignment: Text.AlignHCenter
                    text: root.rawWPM
                }

            }
        }
        Rectangle {
            border.width: 1
            radius: 6
            height: 48
            width: 100
            Column {
                anchors.fill: parent
                Text {
                    id: labelWPM
                    width: parent.width
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 20
                    font.bold: true
                    text: "WPM"
                }
                Text {
                    id: numberWPM
                    width: parent.width
                    horizontalAlignment: Text.AlignHCenter
                    text: root.onlyCorrectWPM
                }

            }
        }

        Rectangle {
            border.width: 1
            radius: 6
            height: 48
            width: labelSymbolTyped.width
            anchors.margins: 3
            Column {
                anchors.fill: parent
                Text {
                    id: labelSymbolTyped
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 20
                    font.bold: true
                    text: "Accuracy"
                }
                Text {
                    id: numberSymbolTyped
                    width: parent.width
                    horizontalAlignment: Text.AlignHCenter
                    text: Number(100. * root.charsCorrect / root.charsTyped).toLocaleString(Qt.locale(), ".3f") + "%"
                    Component.onCompleted: console.log(root.charsCorrect, root.charsTyped, 1. * root.charsCorrect / root.charsTyped)
                }

            }
        }
    }

    ChartView {
        anchors.top: statsRow.bottom
        anchors.topMargin: 4
        anchors.bottom: gameIDview.top
        anchors.bottomMargin: 4
        anchors.left: textScreen.left
        anchors.right: textScreen.right

        antialiasing: true
        legend.visible: true
        LineSeries {
            name: "raw"
            id: wpmSeries
            axisY: ValueAxis {
                min: 0
                max: root.maxChartWPM
                labelFormat: "%.0f"
            }
            axisX: ValueAxis {
                labelFormat: "%.0f"
                tickCount: wpmSeries.count + 1
                min: 1
                max: wpmSeries.count + 1
            }

            VXYModelMapper {
                model: root.chartModel
                xColumn: 0
                yColumn: 1
            }
            Component.onCompleted: console.log("1", count)
        }
        LineSeries {
            name: "wpm"
//            axisY: ValueAxis {
//                min: 0
//                max: root.maxChartWPM
//            }
            VXYModelMapper {
                model: root.chartModel
                xColumn: 0
                yColumn: 2
            }
            Component.onCompleted: console.log("2", count)
        }

        ScatterSeries {
            markerSize: 6
            name: "errors"
            axisYRight: ValueAxis {
                min: 0
                max: root.maxChartErrors + 1
                labelFormat: "%.0f"
                tickCount: root.maxChartErrors + 2
            }
            VXYModelMapper {
                model: root.chartModel
                xColumn: 0
                yColumn: 3
            }
            Component.onCompleted: console.log("3", count)
        }
    }
}
