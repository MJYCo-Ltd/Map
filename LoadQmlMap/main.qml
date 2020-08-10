import QtQuick 2.13
import QtQuick.Window 2.13
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
//        Rectangle
//        {
//            id:add
//            x:10
//            y:10
//            width: 100
//            height: 100
//            color: "red"
//            MouseArea
//            {
//                anchors.fill: parent
//                acceptedButtons: Qt.LeftButton
//                onPressed:
//                {
//                    $app.plotALine();
//                }
//            }
//        }

//        Rectangle
//        {
//            id:clear
//            anchors.left: add.right
//            anchors.leftMargin: 10
//            y:10
//            width: 100
//            height: 100
//            color: "blue"
//            MouseArea
//            {
//                anchors.fill: parent
//                acceptedButtons: Qt.LeftButton
//                onPressed:
//                {
//                    $app.clearPlot();
//                }
//            }
//        }

        id:showOsg
        focus: true
        anchors.fill: parent
        Component.onCompleted:
        {
            $app.setOsgItem(showOsg)
        }
    }
}
