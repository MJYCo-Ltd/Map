import QtQuick 2.13
import QtQuick.Window 2.13
import MyItem 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    OsgItem
    {
        focus: true
        anchors.fill: parent
    }
}
