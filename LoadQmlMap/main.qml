import QtQuick 2.12
import QtQuick.Window 2.12
import MyItem 1.0

Window
{
    id: mainWindow
    property var $app: AppGlobal{}

    visible: true
    width: 1024
    height: 768
    title: qsTr("Hello World")
    property int menuHeight: defaultStyle.menuHeight
    property int menuWidth: defaultStyle.menuWidth
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
    Style{
        id:defaultStyle
    }
    Rectangle{
        id:menuBar
        width: mainWindow.width
        height: menuHeight
        color:"transparent"
        Rectangle
        {
            id:background
            anchors.fill: parent
            opacity: 0.1
            Image{
                anchors.fill: parent
                source: "Image/TalkboxBG.png"
            }
        }
        Rectangle{
            id:marginLeft
            width: (menuBar.width - 4 * menuWidth) / 2
            color:"transparent"
        }
        MenuButton{
            id: btnScenario
            width: menuWidth
            height: menuHeight
            text:qsTr("方案")
            anchors.left: marginLeft.right
            anchors.top: parent.top
            onClicked: {
                scenarioListView.visible = ! scenarioListView.visible
            }
        }
        MenuButton{
            id: btnDataTime
            anchors.left: btnScenario.right
            width: menuWidth
            height: menuHeight
            text:qsTr("日期")
            onClicked: {
                selectDataTime.visible = ! selectDataTime.visible
            }
        }
        MenuButton{
            id: btnMsgBox
            anchors.left: btnDataTime.right
            width: menuWidth
            height: menuHeight
            text:qsTr("消息")
            onClicked: {
                testMsg.visible = ! testMsg.visible
            }
        }
        MenuButton{
            id: btnSystem
            anchors.left: btnMsgBox.right
            width: menuWidth
            height: menuHeight
            text:qsTr("系统")
            onClicked: {
            }
        }
    }
    Rectangle{
        id:menuBarDecoration
        width:mainWindow.width
        height:16
        anchors.top: menuBar.bottom
        color:"transparent"
        Image {
            width:mainWindow.width
            height:16
            source: "Image/Menu-TopLine-Emblem.png"
        }
    }
    ScenarioListView {
        id: scenarioListView
        x: defaultStyle.margin
        y: defaultStyle.margin + menuHeight
        width: 400
        height: mainWindow.height - y
        visible: false
    }
    DateTimeView {
        id:selectDataTime
        x: defaultStyle.margin
        y: defaultStyle.margin + menuHeight
        visible: false
        onDataTime:{
            console.log(value)
        }
    }
    Rectangle{
        id:testMsg
        x:(parent.width - width) / 2
        y:parent.height - height - menuHeight
        width:650
        height:300
        color:"transparent"
        visible: false
        Column
        {
            MsgDone{
                text:" Well done! You successfullyread this important."
            }
            MsgInfo{
                text:"Heads up! This alert needs your attention, but it's not super important."
            }
            MsgAlert{
                text:"Oh snap! Change a few things up and try submitting again."
            }
            MsgWarning{
                text:"Warning! Better check yourself, you're not looking too good."
            }
            MsgGood{
                text:" Well done! You successfully read this important."
            }
        }
    }
}
