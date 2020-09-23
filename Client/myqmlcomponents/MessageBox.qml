import QtQuick 2.12
import QtQuick.Layouts 1.12

RowLayout {
    id: root_row
    property string sender: ""
    property string message: ""
    property string time: ""
    property string fontColor: ""
    property string backColor: ""
    property bool isMy: false

    width: parent.width

    Rectangle {
        id: root_rect

        width: message_text.width + time_text.width + 50
        height: message_text.height + 30
        radius: 25

        Layout.alignment: root_row.isMy ? Qt.AlignLeft : Qt.AlignRight
        Layout.leftMargin: 25
        Layout.rightMargin: 25

        color: backColor

        Text {
            id: message_text

            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 25

            text: root_row.message
            font.pixelSize: 20
            color: root_row.fontColor

            width: paintedWidth > 350 ? 350 : paintedWidth
            wrapMode: Text.WordWrap
        }

        Text {
            id: sender_text
            anchors.bottom: message_text.top
            anchors.left: message_text.left

            text: root_row.sender
            font.underline: true
            font.pixelSize: 10
            color: "gray"
        }

        Text {
            id: time_text
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: message_text.right
            anchors.leftMargin: 10

            text: root_row.time
            font.underline: true
        }
    }
}
