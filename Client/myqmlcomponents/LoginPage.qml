import QtQuick 2.13
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Page {
    id: root_page

    Rectangle {
        id: login_rect
        width: 350
        height: 285
        border.color: "gray"
        border.width: 5
        radius: 30
        anchors.centerIn: parent

        Text {
            id: login_text
            anchors.top: parent.top
            anchors.topMargin: 45
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Sing in"
            font.pixelSize: 25
        }

        TextField {
            id: nickname
            anchors.top: login_text.bottom
            anchors.topMargin: 15
            anchors.horizontalCenter: parent.horizontalCenter
            placeholderText: "Enter your nickname..."
            text: "Default"
        }

        Button {
            id: enter_button
            anchors.top: nickname.bottom
            anchors.topMargin: 15
            anchors.horizontalCenter: nickname.horizontalCenter
            text: "Login"
            enabled: nickname.length

            onClicked: chat_model.joinChat(nickname.text)
        }
    }
}

