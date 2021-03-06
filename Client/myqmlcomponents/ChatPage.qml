﻿import QtQuick 2.13
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

Page {
    id: root_page
    background: Rectangle {
        anchors.fill: parent
        color: "lightgray"
    }

    Button {
        id: exit_button
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 50
        text: "Leave group"
        z: 5       

        onClicked: {
            message_type_select.currentIndex = 0
            chat_model.leaveGroup()
            text_input.clear()
        }
    }

    Text {
        id: nickname_text
        text: "You: " + chat_model.nickname
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 5
        font.pixelSize: 20
    }

    Text {
        id: roomname_text
        text: "Room: " + chat_model.group
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 20
    }

    Text {
        id: users_list_text
        text: "Users list"
        anchors.top: roomname_text.bottom
        anchors.horizontalCenter: roomname_text.horizontalCenter
        font.pixelSize: roomname_mousearea.pressed ? 17 : 15
        font.bold: roomname_mousearea.containsMouse

        MouseArea {
            id: roomname_mousearea
            anchors.fill: parent
            hoverEnabled: true

            onClicked: stack_view.push("UsersListPage.qml")
        }
    }

    Text {
        id: users_count
        text: "Users online: " + users_model.usersOnline
        anchors.horizontalCenter: users_list_text.horizontalCenter
        anchors.top: users_list_text.bottom
        anchors.topMargin: 5
    }

    ListView {
        id: list_view
        anchors.fill: parent
        anchors.bottomMargin: 50
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
            fontColor: font_color
            backColor: back_color
        }
    }

    RowLayout {
        id: root_row
        anchors.bottom: parent.bottom
        width: parent.width

        TextField {
            id: text_input
            height: 50
            Layout.alignment: Qt.AlignCenter
            Layout.leftMargin: 5
            Layout.fillWidth: true
            font.pixelSize: 20

            placeholderText: "Type a message"
        }

        ComboBox {
            id: message_type_select

            model: ListModel {

                ListElement {
                    text: "To all"
                }

                ListElement {
                    text: "To current"
                }
            }
        }

        ComboBox {
            id: reciever_select
            property string selectedNickname: ""
            visible: message_type_select.currentIndex == 1 ? true : false
            model: users_model

            delegate: Rectangle {
                height: 50
                width: parent.width
                color: delegate_mousearea.containsMouse ? "lightgray" : "transparent"

                Text {
                    id: nickname_field
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 15

                    color: color_
                    text: nickname_ == chat_model.nickname ? "You" : nickname_
                    font.pixelSize: 14
                }

                MouseArea {
                    id: delegate_mousearea
                    anchors.fill: parent
                    hoverEnabled: true

                    onClicked: {
                        reciever_select.selectedNickname = nickname_field.text
                        reciever_select.displayText = nickname_field.text
                        reciever_select.update()
                    }
                }
            }
        }

        Rectangle {
            id: send_button
            color: mouse_area.containsMouse ? "green" : "lightgreen"
            width: 50
            height: width
            radius: 25
            Layout.rightMargin: 5

            MouseArea {
                id: mouse_area
                anchors.fill: parent
                enabled: text_input.length > 0

                onPressed: {
                    if (message_type_select.currentIndex == 0)
                        chat_model.sendPublicMsg(text_input.text)

                    else if (reciever_select.selectedNickname.length > 0)
                        chat_model.sendPrivateMsg(reciever_select.selectedNickname, text_input.text)

                    text_input.text = ""
                }
            }
        }
    }
}

