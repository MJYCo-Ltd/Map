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

    ButtonA{
        id: btnScenario
        width: 128
        height: menuHeight
        border.width: 0
        BorderImg2
        {
            color:"transparent"
            titleWidth: 0
            titleHeight: 0
            opacity:  0.2
            margin : 3
            anchors.fill: parent
        }
        text:qsTr("方案管理")
        anchors.left: parent.left
        anchors.top: parent.top
        onClicked: {
            scenarioListView.visible = ! scenarioListView.visible
        }
    }
    ButtonA{
        id: btn2
        anchors.left: btnScenario.right
        width: 128
        height: menuHeight
        border.width: 0
        BorderImg2
        {
            color:"transparent"
            titleWidth: 0
            titleHeight: 0
            opacity:  0.2
            margin : 3
            anchors.fill: parent
        }
        text:qsTr("日期时间设置")
        onClicked: {
            selectDataTime.visible = ! selectDataTime.visible
        }
    }


    ScenarioListView {
        id: scenarioListView
        x:0
        y: menuHeight
        width: 400
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
