import QtQuick 2.13
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Page {
    id: root_page

    SwipeView {
        id: swipe_view
        anchors.centerIn: parent
        interactive: false
        clip: true
        width: 350
        height: 285

        Page {
            Rectangle {
                id: auth_rect
                width: 350
                height: 285
                border.color: "gray"
                border.width: 5
                radius: 30
                Layout.alignment: Qt.AlignCenter


                Text {
                    id: authorize_label
                    anchors.top: parent.top
                    anchors.topMargin: 15
                    anchors.horizontalCenter: parent.horizontalCenter

                    text: "Sign in"
                    font.pixelSize: 25
                }

                TextField {
                    id: auth_nickname
                    anchors.top: authorize_label.bottom
                    anchors.topMargin: 15
                    anchors.horizontalCenter: authorize_label.horizontalCenter

                    text: "root"
                    placeholderText: "Enter your nickname..."
                }

                TextField {
                    id: auth_password
                    anchors.top: auth_nickname.bottom
                    anchors.topMargin: 15
                    anchors.horizontalCenter: auth_nickname.horizontalCenter
                    visible: auth_nickname.text.length

                    echoMode: TextInput.Password
                    text: "root"
                    placeholderText: "Enter password..."
                }

                Button {
                    id: auth_button
                    anchors.top: auth_password.bottom
                    anchors.topMargin: 15
                    anchors.horizontalCenter: auth_password.horizontalCenter

                    text: "Sign in"
                    enabled: auth_nickname.length && auth_password.text.length

                    onClicked: chat_model.signIn(auth_nickname.text, auth_password.text)
                }
            }
        }

        Page {
            Rectangle {
                id: register_rect
                width: 350
                height: 285
                border.color: "gray"
                border.width: 5
                radius: 30
                Layout.alignment: Qt.AlignCenter


                Text {
                    id: register_label
                    anchors.top: parent.top
                    anchors.topMargin: 15
                    anchors.horizontalCenter: parent.horizontalCenter

                    text: "Sign up"
                    font.pixelSize: 25
                }

                TextField {
                    id: register_nickname
                    anchors.top: register_label.bottom
                    anchors.topMargin: 15
                    anchors.horizontalCenter: register_label.horizontalCenter

                    placeholderText: "Enter your nickname..."
                }

                TextField {
                    id: register_password1
                    anchors.top: register_nickname.bottom
                    anchors.topMargin: 15
                    anchors.horizontalCenter: register_nickname.horizontalCenter
                    visible: register_nickname.text.length

                    echoMode: TextInput.Password
                    placeholderText: "Enter password..."
                }

                TextField {
                    id: register_password2
                    anchors.top: register_password1.bottom
                    anchors.topMargin: 15
                    anchors.horizontalCenter: register_password1.horizontalCenter
                    visible: register_password1.text.length

                    echoMode: TextInput.Password
                    placeholderText: "Confirm password..."
                }

                Button {
                    id: register_button
                    anchors.top: register_password2.bottom
                    anchors.topMargin: 15
                    anchors.horizontalCenter: register_password2.horizontalCenter

                    text: register_password1.text.length && register_password1.text == register_password2.text ?
                              "Create": "Passwords do not match"

                    enabled: register_nickname.length &&
                             register_password1.text.length && register_password2.text.length &&
                             register_password1.text == register_password2.text

                    onClicked: chat_model.signUp(register_nickname.text, register_password1.text)
                }
            }
        }
    }

    Button {
        id: auth_register_switch_button
        anchors.top: swipe_view.bottom
        anchors.topMargin: 15
        anchors.horizontalCenter: swipe_view.horizontalCenter

        text: swipe_view.currentIndex == 0 ? "Sign up" : "Sign in"

        onClicked: swipe_view.currentIndex = swipe_view.currentIndex == 0 ? 1 : 0
    }
}

