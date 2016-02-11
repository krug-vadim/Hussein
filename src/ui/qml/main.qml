import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: console.log("Open action triggered");
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }
    TaskView {
        anchors.fill: parent
        TableViewColumn {
            title: "Name"
            role: "display"
            width: 300
        }
        itemDelegate: Item {
            Text {
                anchors.verticalCenter: parent.verticalCenter
                height: parent.height
                /*color: styleData.textColor*/
                /*elide: styleData.elideMode*/
                text: styleData.value
            }
        }
        /*TableViewColumn {
                title: "Name"
                role: "displayRole"
                width: 300
            }
            TableViewColumn {
                title: "Permissions"
                role: "filePermissions"
                width: 100
            }*/
        model: taskModel
    }

}
