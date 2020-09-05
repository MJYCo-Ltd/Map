import QtQuick 2.12
import QtQuick.Window 2.12
import MyItem 1.0

Window
{
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

    Rectangle{
        id: btnScenario
        width: 128
        height: menuHeight
        opacity:0.5
        Text{
            text:"scenario"
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
        opacity:0.5
        Text{
            text:"datetime"
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
