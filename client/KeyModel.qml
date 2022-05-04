import QtQuick 2.15

Item {
    property QtObject firstRowModel: first
    property QtObject secondRowModel: second
    property QtObject thirdRowModel: third
    property QtObject numbersRowModel: numbers

    QtObject {
        id: left
        property color forefinger: "#00b0ec"
        property color middlefinger: "#70c27b"
        property color ringfinger: "#faa61d"
        property color smallfinger: "#d74547"
    }

    ListModel {
        id: numbers
        ListElement { letter: "~"; keycode: Qt.Key_QuoteLeft; clr: "#d74547"}
        ListElement { letter: "1"; keycode: Qt.Key_1; clr: "#d74547"}
        ListElement { letter: "2"; keycode: Qt.Key_2; clr: "#faa61d"}
        ListElement { letter: "3"; keycode: Qt.Key_3; clr: "#70c27b"}
        ListElement { letter: "4"; keycode: Qt.Key_4; clr: "#00b0ec"}
        ListElement { letter: "5"; keycode: Qt.Key_5; clr: "#00b0ec"}
        ListElement { letter: "6"; keycode: Qt.Key_6; clr: "#5985c1"}
        ListElement { letter: "7"; keycode: Qt.Key_7; clr: "#5985c1"}
        ListElement { letter: "8"; keycode: Qt.Key_8; clr: "#97d7f6"}
        ListElement { letter: "9"; keycode: Qt.Key_9; clr: "#ffe8a0"}
        ListElement { letter: "0"; keycode: Qt.Key_0; clr: "#c0e1bf"}
        ListElement { letter: "-"; keycode: Qt.Key_Minus; clr: "#c0e1bf"}
        ListElement { letter: "="; keycode: Qt.Key_Equal; clr: "#c0e1bf"}

    }

    ListModel {
        id:first
        ListElement { letter: "q"; keycode: Qt.Key_Q; clr: "#d74547"}
        ListElement { letter: "w"; keycode: Qt.Key_W; clr: "#faa61d"}
        ListElement { letter: "e"; keycode: Qt.Key_E; clr: "#70c27b"}
        ListElement { letter: "r"; keycode: Qt.Key_R; clr: "#00b0ec"}
        ListElement { letter: "t"; keycode: Qt.Key_T; clr: "#00b0ec"}
        ListElement { letter: "y"; keycode: Qt.Key_Y; clr: "#5985c1"}
        ListElement { letter: "u"; keycode: Qt.Key_U; clr: "#5985c1"}
        ListElement { letter: "i"; keycode: Qt.Key_I; clr: "#97d7f6"}
        ListElement { letter: "o"; keycode: Qt.Key_O; clr: "#ffe8a0"}
        ListElement { letter: "p"; keycode: Qt.Key_P; clr: "#c0e1bf"}
        ListElement { letter: "["; keycode: Qt.Key_BracketLeft; clr: "#c0e1bf"}
        ListElement { letter: "]"; keycode: Qt.Key_BracketRight; clr: "#c0e1bf"}
    }
    ListModel {
        id:second
        ListElement { letter: "a"; keycode: Qt.Key_A; clr: "#d74547"}
        ListElement { letter: "s"; keycode: Qt.Key_S; clr: "#faa61d"}
        ListElement { letter: "d"; keycode: Qt.Key_D; clr: "#70c27b"}
        ListElement { letter: "f"; keycode: Qt.Key_F; clr: "#00b0ec"}
        ListElement { letter: "g"; keycode: Qt.Key_G; clr: "#00b0ec"}
        ListElement { letter: "h"; keycode: Qt.Key_H; clr: "#5985c1"}
        ListElement { letter: "j"; keycode: Qt.Key_J; clr: "#5985c1"}
        ListElement { letter: "k"; keycode: Qt.Key_K; clr: "#97d7f6"}
        ListElement { letter: "l"; keycode: Qt.Key_L; clr: "#ffe8a0"}
        ListElement { letter: ";"; keycode: Qt.Key_Semicolon; clr: "#c0e1bf"}
        ListElement { letter: "'"; keycode: Qt.Key_Apostrophe; clr: "#c0e1bf"}
    }
    ListModel {
        id:third
        ListElement { letter: "z"; keycode: Qt.Key_Z; clr: "#d74547"}
        ListElement { letter: "x"; keycode: Qt.Key_X; clr: "#faa61d"}
        ListElement { letter: "c"; keycode: Qt.Key_C; clr: "#70c27b"}
        ListElement { letter: "v"; keycode: Qt.Key_V; clr: "#00b0ec"}
        ListElement { letter: "b"; keycode: Qt.Key_B; clr: "#00b0ec"}
        ListElement { letter: "n"; keycode: Qt.Key_N; clr: "#5985c1"}
        ListElement { letter: "m"; keycode: Qt.Key_M; clr: "#5985c1"}
        ListElement { letter: ","; keycode: Qt.Key_Comma; clr: "#97d7f6"}
        ListElement { letter: "."; keycode: Qt.Key_Period; clr: "#ffe8a0"}
        ListElement { letter: "/"; keycode: Qt.Key_Slash; clr: "#c0e1bf"}
    }
}
