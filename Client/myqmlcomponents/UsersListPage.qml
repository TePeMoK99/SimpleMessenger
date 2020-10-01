import QtQuick 2.13
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Page {
    id: root_page

    Button {
        anchors.left: parent.left
        anchors.top: parent.top
        text: "Back"

        onClicked: {
            swipe_view.decrementCurrentIndex()
        }
    }

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 5
        text: "Users online: " + users_model.usersOnline
    }

    ListView {
        id: list_model
        anchors.fill: parent
        anchors.topMargin: 50
        clip: true
        spacing: 10
        model: users_model

        delegate: UserBox {
            nickname: nickname_
            color: color_
        }
    }
}

