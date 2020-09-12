import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12

RowLayout {
    id: root_row
    property string sender: ""
    property string message: ""
    property string time: ""
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

        color: root_row.isMy ? "#9ceca2" : "#5998ee"

        Text {
            id: message_text

            text: root_row.message
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 25
            font.pixelSize: 20

            width: paintedWidth > 350 ? 350 : paintedWidth
            wrapMode: Text.WordWrap
        }

        Text {
            id: sender_text
            text: root_row.sender
            anchors.bottom: message_text.top
            anchors.left: message_text.left
            font.underline: true
        }

        Text {
            id: time_text
            text: root_row.time
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: message_text.right
            anchors.leftMargin: 10
            font.underline: true
        }
    }
}
