import QtQuick 2.13
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import "myqmlcomponents/"

ApplicationWindow {
    id: root
    property bool logged: false
    visible: true
    width: 600
    height: 800
    color: "lightgray"


    Text {
        id: roomname_text
        anchors.bottom: list_view.top
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Room: 09-841 v.1"
        font.pixelSize: 20
        font.bold: true
        visible: root.logged
    }

    ListView {
        id: list_view
        anchors.fill: parent
        anchors.bottomMargin: 75
        anchors.topMargin: 25
        visible: root.logged

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

//        width: parent.width - 100
        height: 50
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: 12.5
        anchors.right: send_button.left
        font.pixelSize: 20
        visible: root.logged

        placeholderText: "Type a message"


    }

    Rectangle {
        id: send_button
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 12.5
        visible: root.logged

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
        visible: !root.logged
        placeholderText: "Enter your nickname..."
    }

//    TextField {
//        id: port_field
//        anchors.top: nickname_field.bottom
//        anchors.horizontalCenter: nickname_field.horizontalCenter
//        visible: !root.logged
//        placeholderText: "Enter port..."
//    }

    Button {
        id: enter_button
        anchors.top: nickname_field.bottom
        anchors.horizontalCenter: nickname_field.horizontalCenter
        visible: !root.logged
        text: "Login"
        enabled: nickname_field.length

        onClicked: {
            ChatModel.joinChat(nickname_field.text)
            root.logged = true
        }
    }    
}
