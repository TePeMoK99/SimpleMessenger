import QtQuick 2.13
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import "myqmlcomponents/"

ApplicationWindow {
    id: root
    visible: true
    width: 600
    height: 800
    color: "lightgray"

    Button {
        id: exit_button
        anchors.right: parent.right
        anchors.top: parent.top
        text: "X"
        visible: ChatModel.isAuth

        onClicked: ChatModel.leftChat()
    }

    Text {
        id: roomname_text
        anchors.bottom: list_view.top
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Room: 09-841 v.1"
        font.pixelSize: 20
        font.bold: true
        visible: ChatModel.isAuth
    }

    ListView {
        id: list_view
        anchors.fill: parent
        anchors.bottomMargin: 75
        anchors.topMargin: 25
        visible: ChatModel.isAuth

        spacing: 15
        clip: true
        verticalLayoutDirection: ListView.BottomToTop

        model: ChatModel

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
        visible: ChatModel.isAuth

        placeholderText: "Type a message"
    }

    Rectangle {
        id: send_button
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 12.5
        visible: ChatModel.isAuth

        color: mouse_area.containsMouse ? "green" : "lightgreen"
        width: 50
        height: width
        radius: 25

        MouseArea {
            id: mouse_area
            anchors.fill: parent
            enabled: text_input.length > 0

            onPressed: {
                ChatModel.sendPublicMessage(text_input.text)
                text_input.text = ""
            }
        }
    }

    TextField {
        id: nickname_field
        anchors.centerIn: parent
        visible: !ChatModel.isAuth
        placeholderText: "Enter your nickname..."
    }

    Button {
        id: enter_button
        anchors.top: nickname_field.bottom
        anchors.horizontalCenter: nickname_field.horizontalCenter
        visible: !ChatModel.isAuth
        text: "Login"
        enabled: nickname_field.length

        onClicked: {
            ChatModel.joinChat(nickname_field.text)
            visible: ChatModel.isAuth
        }
    }    
}
