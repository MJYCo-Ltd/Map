import QtQuick 2.12
import QtQuick.Window 2.12
import MyItem 1.0

Window
{
    property var $app: AppGlobal{}
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Row
    {
    OsgItem
    {
        id:showOsg
        focus: true
        anchors.fill: parent
        Component.onCompleted:
        {
            $app.setOsgItem(showOsg)
        }
    }
    ListView {
        id: view
        width: 200
        height: parent.height

        model: scenarioManager.scenarios
        delegate: Rectangle {
            width: view.width
            height: 40
            border.color: Qt.darker(color)

            Text {
                anchors.left: parent.left
                text: modelData
            }
        }
    }
    }
}
