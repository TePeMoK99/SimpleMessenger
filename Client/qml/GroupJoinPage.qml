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

    SwipeView {
        id: swipe_view
        anchors.centerIn: parent
        interactive: false
        clip: true
        width: 350
        height: 285

        Page {
            Rectangle {
                id: join_rect
                width: 350
                height: 285
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
                    id: group_name
                    anchors.top: join_group.bottom
                    anchors.topMargin: 15
                    anchors.horizontalCenter: join_group.horizontalCenter

                    text: "Main"
                    placeholderText: "Enter group name..."
                }

                TextField {
                    id: password
                    anchors.top: group_name.bottom
                    anchors.topMargin: 15
                    anchors.horizontalCenter: group_name.horizontalCenter
                    visible: group_name.text.length

                    echoMode: TextInput.Password
                    text: "0000"
                    placeholderText: "Enter password..."
                }

                Button {
                    id: join_button
                    anchors.top: password.bottom
                    anchors.topMargin: 15
                    anchors.horizontalCenter: password.horizontalCenter

                    text: "Join"
                    enabled: group_name.length && password.text.length

                    onClicked: chat_model.joinGroup(group_name.text, password.text)
                }
            }
        }

        Page {
            Rectangle {
                id: create_rect
                width: 350
                height: 285
                border.color: "gray"
                border.width: 5
                radius: 30
                Layout.alignment: Qt.AlignCenter


                Text {
                    id: create_group_label
                    anchors.top: parent.top
                    anchors.topMargin: 15
                    anchors.horizontalCenter: parent.horizontalCenter

                    text: "Create group"
                    font.pixelSize: 25
                }

                TextField {
                    id: create_group_name
                    anchors.top: create_group_label.bottom
                    anchors.topMargin: 15
                    anchors.horizontalCenter: create_group_label.horizontalCenter

                    placeholderText: "Enter group name..."
                }

                TextField {
                    id: password_create
                    anchors.top: create_group_name.bottom
                    anchors.topMargin: 15
                    anchors.horizontalCenter: create_group_name.horizontalCenter
                    visible: create_group_name.text.length

                    echoMode: TextInput.Password
                    placeholderText: "Enter password..."
                }

                TextField {
                    id: password_repeat
                    anchors.top: password_create.bottom
                    anchors.topMargin: 15
                    anchors.horizontalCenter: password_create.horizontalCenter
                    visible: password_create.text.length

                    echoMode: TextInput.Password
                    placeholderText: "Confirm password..."
                }

                Button {
                    id: create_button
                    anchors.top: password_repeat.bottom
                    anchors.topMargin: 15
                    anchors.horizontalCenter: password_repeat.horizontalCenter

                    text: password_create.text.length && password_create.text == password_repeat.text ?
                              "Create": "Passwords do not match"

                    enabled: create_group_name.length &&
                             password_create.text.length && password_repeat.text.length &&
                             password_create.text == password_repeat.text

                    onClicked: chat_model.createGroup(create_group_name.text, password_create.text)
                }
            }
        }
    }

    Button {
        id: join_create_button
        anchors.top: swipe_view.bottom
        anchors.topMargin: 15
        anchors.horizontalCenter: swipe_view.horizontalCenter

        text: swipe_view.currentIndex == 0 ? "Create group" : "Join group"

        onClicked: swipe_view.currentIndex = swipe_view.currentIndex == 0 ? 1 : 0
    }

    Button {
        id: exit_button
        anchors.left: parent.left
        anchors.top: parent.top
        text: "Exit"

        onClicked: chat_model.leftChat()
    }
}
