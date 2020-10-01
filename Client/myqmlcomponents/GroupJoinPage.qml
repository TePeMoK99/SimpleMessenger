import QtQuick 2.13
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Page {
    id: root_page

    Text {
        id: nickname
        text: chat_model.nickname
        anchors.top: parent.top
        anchors.topMargin: 15
        anchors.horizontalCenter: parent.horizontalCenter
    }

    ColumnLayout {
        id: column_layout
        anchors.fill: parent

        Rectangle {
            id: join_rect
            width: 350
            height: 230
            border.color: "gray"
            border.width: 5
            radius: 30
            Layout.alignment: Qt.AlignCenter


            Text {
                id: join_group
                anchors.top: parent.top
                anchors.topMargin: 15
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Join group"
                font.pixelSize: 25
            }

            TextField {
                id: group_name_field
                anchors.top: join_group.bottom
                anchors.topMargin: 15
                anchors.horizontalCenter: join_group.horizontalCenter
                text: "Main"
                placeholderText: "Enter group name..."
            }

            TextField {
                id: password_field
                anchors.top: group_name_field.bottom
                anchors.topMargin: 15
                anchors.horizontalCenter: group_name_field.horizontalCenter
                visible: group_name_field.text.length

                echoMode: TextInput.Password
                text: "0000"
                placeholderText: "Enter password..."
            }

            Button {
                id: join_button
                anchors.top: password_field.bottom
                anchors.topMargin: 15
                anchors.horizontalCenter: password_field.horizontalCenter
                text: "Join"
                enabled: group_name_field.length && password_field.text.length

                onClicked: chat_model.joinGroup(group_name_field.text, password_field.text)
            }
        }

        Rectangle {
            id: create_rect
            width: 350
            height: 285
            border.color: "gray"
            border.width: 5
            radius: 30
            Layout.alignment: Qt.AlignCenter


            Text {
                id: create_group
                anchors.top: parent.top
                anchors.topMargin: 15
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Create group"
                font.pixelSize: 25
            }

            TextField {
                id: create_name_field
                anchors.top: create_group.bottom
                anchors.topMargin: 15
                anchors.horizontalCenter: create_group.horizontalCenter
                placeholderText: "Enter group name..."
            }

            TextField {
                id: password1_field
                anchors.top: create_name_field.bottom
                anchors.topMargin: 15
                anchors.horizontalCenter: create_name_field.horizontalCenter
                visible: create_name_field.text.length

                echoMode: TextInput.Password
                placeholderText: "Enter password..."
            }

            TextField {
                id: password2_field
                anchors.top: password1_field.bottom
                anchors.topMargin: 15
                anchors.horizontalCenter: password1_field.horizontalCenter
                visible: password1_field.text.length

                echoMode: TextInput.Password
                placeholderText: "Confirm password..."
            }

            Button {
                id: create_button
                anchors.top: password2_field.bottom
                anchors.topMargin: 15
                anchors.horizontalCenter: password2_field.horizontalCenter
                text: password1_field.text.length && password1_field.text == password2_field.text ?
                          "Create": "Passwords do not match"
                enabled: create_name_field.length &&
                         password1_field.text.length && password2_field.text.length &&
                         password1_field.text == password2_field.text

                onClicked: chat_model.createGroup(create_name_field.text, password1_field.text)
            }
        }
    }

    Button {
        id: exit_button
        anchors.left: parent.left
        anchors.top: parent.top
        text: "Exit"

        onClicked: chat_model.leftChat()
    }
}
