import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

ApplicationWindow {
    visible: true
    width: 640
    height: 480

    Button {
        anchors.centerIn: parent;
        text: "Connect"

        onPressed: Client.connectToServer()
    }
}
