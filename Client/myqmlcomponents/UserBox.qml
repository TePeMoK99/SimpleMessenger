import QtQuick 2.12
import QtQuick.Layouts 1.12

RowLayout {
    id: root_row
    property string nickname: ""
    property string color: ""
    width: parent.width

    Text {
        Layout.alignment: Qt.AlignLeft
        Layout.leftMargin: 25

        text: root_row.nickname
        font.pixelSize: 20
        color: parent.color
    }
}
