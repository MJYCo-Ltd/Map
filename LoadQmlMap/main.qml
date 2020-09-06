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

    property int menuHeight: 42
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
        id: btnScenario
        width: 128
        height: menuHeight
        border
        {
            width : 1
            color : defaultStyle.secondaryColor
        }
        color: defaultStyle.backgroundColor
        opacity:0.6

        Text{
            anchors.centerIn: parent
            text:qsTr("方案管理")
            color:defaultStyle.fontColor
        }
        anchors.left: parent.left
        anchors.top: parent.top
        MouseArea {
            anchors.fill: parent
            onClicked: {
                scenarioListView.visible = ! scenarioListView.visible
            }
        }
    }
    Rectangle{
        id: btn2
        anchors.left: btnScenario.right
        width: 128
        height: menuHeight
        border
        {
            width : 2
            color : defaultStyle.secondaryColor
        }
        color: defaultStyle.backgroundColor
        opacity:0.6
        Text{
            anchors.centerIn: parent
            text:qsTr("日期时间设置")
            color:defaultStyle.fontColor
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                selectDataTime.visible = ! selectDataTime.visible
            }
        }
    }


    ScenarioListView {
        id: scenarioListView
        x:0
        y: menuHeight
        width: 500
        height: mainWindow.height - menuHeight
        visible: false
    }

    DateTimeView {
        id:selectDataTime
        x: 0
        y: menuHeight
        visible: false
        onDataTime:{
            console.log(value)
        }
    }

}
