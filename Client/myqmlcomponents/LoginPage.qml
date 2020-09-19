import QtQuick 2.13
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Page {
    id: root_page
//    anchors.fill: parent


    TextField {
        id: nickname_field
        anchors.centerIn: parent
        placeholderText: "Enter your nickname..."
    }

    Button {
        id: enter_button
        anchors.top: nickname_field.bottom
        anchors.horizontalCenter: nickname_field.horizontalCenter
        text: "Login"
        enabled: nickname_field.length

        onClicked: chat_model.joinChat(nickname_field.text)
    }
}
