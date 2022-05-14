import QtQuick 2.15
import fasttyping.keyboard 1.0

Item {
    id:root
    objectName: "keyboard"
    width: parent.width
    height: parent.height
    property KeyboardModel keyModel

    function pressKey(key) {
        console.log("pressed key", key);
        console.log("debug text", root.keyModel.firstRowModel);

        if (pimpl.shiftModifier < 0) {
            releaseKey(Qt.Key_Shift);
            pimpl.shiftModifier = 0;
        }
        if (key === Qt.Key_Shift) {
            pimpl.shiftModifier++;

        }
        if (key === Qt.Key_CapsLock) {
            pimpl.capsModifier = !pimpl.capsModifier
        }

        pressed(key);
    }

    function releaseKey(key) {
        console.log("released key", key);
        if (key === Qt.Key_Shift) {
            pimpl.shiftModifier--;
        }
        released(key);
    }

    signal pressed(int key)
    signal released(int key)

    QtObject {
        id:pimpl
        property int shiftModifier: 0
        property bool capsModifier: false
        property int verticalSpacing: keyboard.height / 100
        property int horizontalSpacing: verticalSpacing
        property int buttonWidth:  (keyboard.width-column.anchors.margins)/15 - horizontalSpacing
        property int rowHeight: buttonWidth
    }

    Component {
        id: keyButtonDelegate
        KeyboardButton {
            id: button
            width: pimpl.buttonWidth
            height: pimpl.rowHeight
            text: (pimpl.capsModifier ? ((!pimpl.shiftModifier && keycode == shiftKeycode) || (pimpl.shiftModifier && keycode != shiftKeycode) ? shiftLetter : letter) :
                                        (pimpl.shiftModifier ? shiftLetter : letter))
            inputPanel: root
            key: keycode
            color: clr
        }
    }
    Rectangle {
        //width: pimpl.buttonWidth * 15 + pimpl.horizontalSpacing * 14
        //height: pimpl.rowHeight * 6 + pimpl.verticalSpacing * 5
        id:keyboard
        color: "white"
        anchors.fill: parent;
        MouseArea {
            anchors.fill: parent
        }

        Column {
            id:column
            anchors.margins: 5
            anchors.fill: parent
            spacing: pimpl.verticalSpacing
            Item {
                height: pimpl.rowHeight
                width:parent.width
                Row {
                    height: pimpl.rowHeight
                    spacing: pimpl.horizontalSpacing
                    anchors.left:parent.left
                    Repeater {
                        model: root.keyModel.numbersRowModel
                        delegate: keyButtonDelegate
                    }
                }
                KeyboardButton {
                    id: backspaceKey
                    color: "#c0e1bf"
                    anchors.right: parent.right
                    width: 2*pimpl.buttonWidth
                    height: pimpl.rowHeight
                    text: "\x7F"
                    key: Qt.Key_Backspace
                    inputPanel: root
                }
            }
            Item {
                height: pimpl.rowHeight
                width:parent.width
                KeyboardButton {
                    id: tabKey
                    color: "#d74547"
                    anchors.left: parent.left
                    width: 1.5*pimpl.buttonWidth
                    height: pimpl.rowHeight
                    text: "\x09"
                    key: Qt.Key_Tab
                    displayText: "\u21B9"
                    inputPanel: root
                }
                Row {
                    height: pimpl.rowHeight
                    spacing: pimpl.horizontalSpacing
                    anchors.horizontalCenter:parent.horizontalCenter
                    Repeater {
                        model: root.keyModel.firstRowModel
                        delegate: keyButtonDelegate
                    }
                }
                KeyboardButton {
                    id: backslashKey
                    color: "#c0e1bf"
                    anchors.right: parent.right
                    width: 1.5*pimpl.buttonWidth
                    height: pimpl.rowHeight
                    text: "\\"
                    key: Qt.Key_Backslash
                    inputPanel: root
                }
            }
            Item {
                height: pimpl.rowHeight
                width:parent.width
                KeyboardButton {
                    id: capsLock
                    color: (pimpl.capsModifier)? "#1e6fa7": "#d74547"
                    anchors.left: parent.left
                    width: 1.75*pimpl.buttonWidth
                    height: pimpl.rowHeight
                    text: "Caps Lock"
                    inputPanel: root
                    key: Qt.Key_CapsLock
                }
                Row {
                    height: pimpl.rowHeight
                    spacing: pimpl.horizontalSpacing
                    anchors.left: capsLock.right
                    anchors.margins: 2 * pimpl.horizontalSpacing
                    /*anchors.horizontalCenter:parent.horizontalCenter*/
                    Repeater {
                        model: root.keyModel.secondRowModel
                        delegate: keyButtonDelegate
                    }
                }
                KeyboardButton {
                    anchors.right:parent.right
                    id: enterKey
                    color: "#c0e1bf"
                    width: 2.25*pimpl.buttonWidth
                    height: pimpl.rowHeight
                    displayText: "Enter"
                    text: "\n"
                    inputPanel: root
                    key: Qt.Key_Return
                }
            }
            Item {
                height: pimpl.rowHeight
                width:parent.width
                KeyboardButton {
                    id: leftShiftKey
                    color: (pimpl.shiftModifier)? "#1e6fa7": "#d74547"
                    anchors.left: parent.left
                    width: 2.25*pimpl.buttonWidth
                    height: pimpl.rowHeight
                    text: "\u21e7 Shift"
                    key: Qt.Key_Shift
                    inputPanel: root
                }
                Row {
                    height: pimpl.rowHeight
                    spacing: pimpl.horizontalSpacing
                    anchors.left: leftShiftKey.right
                    anchors.margins: 2 * pimpl.horizontalSpacing
                    Repeater {
                        anchors.horizontalCenter: parent.horizontalCenter
                        model: root.keyModel.thirdRowModel
                        delegate: keyButtonDelegate
                    }
                }
                KeyboardButton {
                    id: rightShiftKey
                    color: (pimpl.shiftModifier)? "#1e6fa7": "#c0e1bf"
                    anchors.right: parent.right
                    width: 2.75*pimpl.buttonWidth
                    height: pimpl.rowHeight
                    text: "\u21e7 Shift"
                    key: Qt.Key_Shift
                    inputPanel: root
                }

            }
            Item {
                height: pimpl.rowHeight
                width:parent.width
                Row {
                    height: pimpl.rowHeight
                    spacing: pimpl.horizontalSpacing
                    anchors.left:parent.left
                    KeyboardButton {
                        id: leftCtrlKey
                        color: "#d74547"
                        width: 1.25*pimpl.buttonWidth
                        height: pimpl.rowHeight
                        text: "Ctrl"
                        key: Qt.Key_Control
                        inputPanel: root
                    }
                    KeyboardButton {
                        id: leftWinKey
                        color: "#bfcbe6"
                        width: 1.25*pimpl.buttonWidth
                        height: pimpl.rowHeight
                        text: "Win"
                        key: Qt.Key_Meta
                        inputPanel: root
                    }
                    KeyboardButton {
                        id: leftAltKey
                        color: "#bfcbe6"
                        width: 1.25*pimpl.buttonWidth
                        height: pimpl.rowHeight
                        text: "Alt"
                        key: Qt.Key_Alt
                        inputPanel: root
                    }
                    KeyboardButton {
                        id: spaceKey
                        color: "#bfcbe6"
                        width: 6.63*pimpl.buttonWidth
                        height: pimpl.rowHeight
                        text: " "
                        key: Qt.Key_Space
                        inputPanel: root
                    }
                    KeyboardButton {
                        id: rightAltKey
                        color: "#bfcbe6"
                        width: 1.25*pimpl.buttonWidth
                        height: pimpl.rowHeight
                        text: "Alt"
                        key: Qt.Key_Alt
                        inputPanel: root
                    }
                    KeyboardButton {
                        id: rightWinKey
                        color: "#bfcbe6"
                        width: 1.25*pimpl.buttonWidth
                        height: pimpl.rowHeight
                        text: "Win"
                        key: Qt.Key_Meta
                        inputPanel: root
                    }
                    KeyboardButton {
                        id: menuKey
                        color: "#bfcbe6"
                        width: 1.25*pimpl.buttonWidth
                        height: pimpl.rowHeight
                        text: "Menu"
                        key: Qt.Key_Menu
                        inputPanel: root
                    }
                    KeyboardButton {
                        id: rightCtrlKey
                        color: "#bfcbe6"
                        width: 1.25*pimpl.buttonWidth
                        height: pimpl.rowHeight
                        text: "Ctrl"
                        key: Qt.Key_Control
                        inputPanel: root
                    }
                }
            }
        }
    }
}
