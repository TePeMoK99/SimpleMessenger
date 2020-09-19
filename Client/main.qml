import QtQuick 2.13
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import ChatModel 1.0
import UserListModel 1.0
import "myqmlcomponents/"

ApplicationWindow {
    id: root
    visible: true
    width: 600
    height: 800

    ChatModel {
        id:  chat_model

        onIsAuthChanged: {
            if (isAuth) {
                swipe_view.setCurrentIndex(1)
            }
            else {
                swipe_view.setCurrentIndex(0)
            }
        }
    }

    UserListModel {
        id: users_model

        onModelCreated: {
            setTcpClient(chat_model.getTcpClient())
        }
    }

    SwipeView {
        id: swipe_view
        anchors.fill: parent
        clip: false
        orientation: Qt.Horizontal
        interactive: false

        LoginPage {

        }

        ChatPage {

        }

        UsersListPage {

        }
    }
}
