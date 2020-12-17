import QtQuick 2.12
import QtQuick.Window 2.12
import MyItem 1.0
import QtQml 2.0

Window
{
    id: mainWindow
    property var $app: AppGlobal{}

    visible: true
    width: 1024
    height: 768
    title: qsTr("Hello World")

    OsgItem
    {
        type: OsgItem.Item_3DMAP
        id:showOsg
        focus: true
        anchors.fill: parent
    }

    Component.onCompleted:
    {
        $app.setOsgItem(showOsg)
    }
}
