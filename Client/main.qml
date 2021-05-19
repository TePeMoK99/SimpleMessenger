import QtQuick 2.13
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import ChatModel 1.0
import UserListModel 1.0
import "qml/"

ApplicationWindow {
    id: root
    visible: true
    width: 600
    height: 800

    ChatModel {
        id:  chat_model

        onIsAuthChanged: {
            if (isAuth && isJoined) {
                stack_view.clear()
                stack_view.push("myqmlcomponents/ChatPage.qml")
            }
            else if (isAuth && !isJoined) {
                stack_view.clear()
                stack_view.push("myqmlcomponents/GroupJoinPage.qml")
            }
            else {
                stack_view.clear()
                stack_view.push("myqmlcomponents/LoginPage.qml")
            }
        }

        onIsJoinedChanged: {
            if (isAuth && isJoined) {
                stack_view.clear()
                stack_view.push("myqmlcomponents/ChatPage.qml")
            }
            else if (isAuth && !isJoined) {
                stack_view.clear()
                stack_view.push("myqmlcomponents/GroupJoinPage.qml")
            }
            else {
                stack_view.clear()
                stack_view.push("myqmlcomponents/LoginPage.qml")
            }
        }
    }

    UserListModel {
        id: users_model
    }

    StackView {
        id: stack_view
        anchors.fill: parent
        clip: false
        initialItem: LoginPage {}
    }
}
