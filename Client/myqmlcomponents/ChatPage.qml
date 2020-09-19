import QtQuick 2.13
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Page {
    id: root_page
    background: Rectangle {
        anchors.fill: parent
        color: "lightgray"
    }

    Button {
        id: exit_button
        anchors.right: parent.right
        anchors.top: parent.top
        text: "X"

        onClicked: chat_model.leftChat()
    }

    Text {
        id: roomname_text
        text: "Room: 09-841 v.1"
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 20
    }

    Text {
        id: users_list_text
        text: "Users list"
        anchors.top: roomname_text.bottom
        anchors.horizontalCenter: roomname_text.horizontalCenter
        font.pixelSize: 15
        font.bold: roomname_mousearea.containsMouse

        MouseArea {
            id: roomname_mousearea
            anchors.fill: parent

            onClicked: {
                swipe_view.setCurrentIndex(2)
            }
        }
    }

    ListView {
        id: list_view
        anchors.fill: parent
        anchors.bottomMargin: 75
        anchors.topMargin: 50

        spacing: 15
        clip: true
        verticalLayoutDirection: ListView.BottomToTop

        model: chat_model

        delegate: MessageBox {
            isMy: isMy_
            sender: sender_
            message: message_
            time: time_
            color: color_
        }
    }

    TextField {
        id: text_input

        height: 50
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: 12.5
        anchors.right: send_button.left
        font.pixelSize: 20

        placeholderText: "Type a message"
    }

    Rectangle {
        id: send_button
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 12.5

        color: mouse_area.containsMouse ? "green" : "lightgreen"
        width: 50
        height: width
        radius: 25

        MouseArea {
            id: mouse_area
            anchors.fill: parent
            enabled: text_input.length > 0

            onPressed: {
                chat_model.sendPublicMessage(text_input.text)
                text_input.text = ""
            }
        }
    }
}
