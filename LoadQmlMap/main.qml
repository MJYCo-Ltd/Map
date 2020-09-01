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
}
