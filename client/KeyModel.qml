import QtQuick 2.15

Item {
    property QtObject firstRowModel: first
    property QtObject secondRowModel: second
    property QtObject thirdRowModel: third
    property QtObject numbersRowModel: numbers

    ListModel {
        id: numbers
        ListElement { letter: "~"; keycode: Qt.Key_QuoteLeft}
        ListElement { letter: "1"; keycode: Qt.Key_1}
        ListElement { letter: "2"; keycode: Qt.Key_2}
        ListElement { letter: "3"; keycode: Qt.Key_3}
        ListElement { letter: "4"; keycode: Qt.Key_4}
        ListElement { letter: "5"; keycode: Qt.Key_5}
        ListElement { letter: "6"; keycode: Qt.Key_6}
        ListElement { letter: "7"; keycode: Qt.Key_7}
        ListElement { letter: "8"; keycode: Qt.Key_8}
        ListElement { letter: "9"; keycode: Qt.Key_9}
        ListElement { letter: "0"; keycode: Qt.Key_0}
        ListElement { letter: "-"; keycode: Qt.Key_Minus}
        ListElement { letter: "="; keycode: Qt.Key_Equal}

    }

    ListModel {
        id:first
        ListElement { letter: "q"; keycode: Qt.Key_Q}
        ListElement { letter: "w"; keycode: Qt.Key_W}
        ListElement { letter: "e"; keycode: Qt.Key_E}
        ListElement { letter: "r"; keycode: Qt.Key_R}
        ListElement { letter: "t"; keycode: Qt.Key_T}
        ListElement { letter: "y"; keycode: Qt.Key_Y}
        ListElement { letter: "u"; keycode: Qt.Key_U}
        ListElement { letter: "i"; keycode: Qt.Key_I}
        ListElement { letter: "o"; keycode: Qt.Key_O}
        ListElement { letter: "p"; keycode: Qt.Key_P}
        ListElement { letter: "["; keycode: Qt.Key_BracketLeft}
        ListElement { letter: "]"; keycode: Qt.Key_BracketRight}
    }
    ListModel {
        id:second
        ListElement { letter: "a"; keycode: Qt.Key_A}
        ListElement { letter: "s"; keycode: Qt.Key_S}
        ListElement { letter: "d"; keycode: Qt.Key_D}
        ListElement { letter: "f"; keycode: Qt.Key_F}
        ListElement { letter: "g"; keycode: Qt.Key_G}
        ListElement { letter: "h"; keycode: Qt.Key_H}
        ListElement { letter: "j"; keycode: Qt.Key_J}
        ListElement { letter: "k"; keycode: Qt.Key_K}
        ListElement { letter: "l"; keycode: Qt.Key_L}
        ListElement { letter: ";"; keycode: Qt.Key_Semicolon}
        ListElement { letter: "'"; keycode: Qt.Key_Apostrophe}
    }
    ListModel {
        id:third
        ListElement { letter: "z"; keycode: Qt.Key_Z}
        ListElement { letter: "x"; keycode: Qt.Key_X}
        ListElement { letter: "c"; keycode: Qt.Key_C}
        ListElement { letter: "v"; keycode: Qt.Key_V}
        ListElement { letter: "b"; keycode: Qt.Key_B}
        ListElement { letter: "n"; keycode: Qt.Key_N}
        ListElement { letter: "m"; keycode: Qt.Key_M}
        ListElement { letter: ","; keycode: Qt.Key_Comma}
        ListElement { letter: "."; keycode: Qt.Key_Period}
        ListElement { letter: "/"; keycode: Qt.Key_Slash}
    }
}
