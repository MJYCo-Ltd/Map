import QtQuick 2.12
import QtQuick.Window 2.12
import MyItem 1.0

Window
{
    id: mainWindow
    visible: true
    x:0
    y:0
    flags: Qt.Window
    //width: 1920
    //height: 1080
    title: qsTr("Test")
    property var $app: AppGlobal{}

    OsgItem{
        type: OsgItem.Item_3DMAP
        id:showOsg
        frameRate: 100
        focus: true
        anchors.fill: parent
        Component.onCompleted:{
            $app.setOsgItem(showOsg)
            mainWindow.showMaximized();
        }
    }
}
