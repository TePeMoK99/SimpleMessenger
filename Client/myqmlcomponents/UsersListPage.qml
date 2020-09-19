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
            swipe_view.setCurrentIndex(1)
        }
    }

    ListView {
        id: list_model
        anchors.fill: parent
        anchors.topMargin: 25
        clip: false
        spacing: 10
        model: users_model

        delegate: Text {
            text: nickname_
        }
    }
}

