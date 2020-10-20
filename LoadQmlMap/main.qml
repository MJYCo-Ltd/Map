import QtQuick 2.12
import QtQuick.Window 2.12
import MyItem 1.0

Window
{
    id: mainWindow
    visibility : Window.FullScreen
    property var $app: AppGlobal{}

    visible: true
    width: 1024
    height: 768
    title: qsTr("Hello World")
    property int menuHeight: defaultStyle.menuHeight
    property int menuWidth: defaultStyle.menuWidth
    ScenarioManager{
        id:scenarioManager
    }
    AreaPlanManager{
        id:areaPlanManager
        //onCompleted{
        //    scenarioManager.addItem(areaPlanManager)
        //}
    }
    AreaPlan{
        id:areaPlan
    }

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
            opacity: 0.15
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
            text:qsTr("WELCOME")
            anchors.left: marginLeft.right
            anchors.top: parent.top
            onClicked: {
                welcome.visible = ! welcome.visible
            }
        }
        MenuButton{
            id: btnDataTime
            anchors.left: btnScenario.right
            width: menuWidth
            height: menuHeight
            text:qsTr("DATE")
            onClicked: {
                selectDataTime.visible = ! selectDataTime.visible
            }
        }
        MenuButton{
            id: btnMsgBox
            anchors.left: btnDataTime.right
            width: menuWidth
            height: menuHeight
            text:qsTr("MESSAGE")
            onClicked: {
                testMsg.visible = ! testMsg.visible
            }
        }
        MenuButton{
            id: btnSystem
            anchors.left: btnMsgBox.right
            width: menuWidth
            height: menuHeight
            text:qsTr("SYSTEM")
            onClicked: {
                Qt.quit()
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
    Welcome {
        id: welcome
        visible: false
    }

    ListView{
        id:areaPlanLayerListView
        x: defaultStyle.margin
        y: defaultStyle.margin + menuHeight
        width: 400
        height: mainWindow.height - y - defaultStyle.margin
        visible : false
        model:areaPlan.layers
        delegate: Rectangle {
            width:400
            height: 48
            color: "transparent"
            border.color: defaultStyle.borderColor
            border.width: 1
            Rectangle{
                anchors.fill: parent
                opacity: 0.15
                Image{
                    anchors.fill: parent
                    source: "Image/TalkboxBG.png"
                }
            }
            Text{
                color: defaultStyle.fontColorGold
                font.family: defaultStyle.fontFamily
                font.pointSize: defaultStyle.fontSize
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                anchors.fill: parent
                text: modelData.name
            }
        }
    }
    ListView{
        x: parent.width - width - defaultStyle.margin
        y: defaultStyle.margin + menuHeight
        width: 400
        height: mainWindow.height - y - defaultStyle.margin
        model:areaPlanManager.itemList
        delegate: Rectangle {
            width:400
            height: 48
            color: "transparent"
            border.color: defaultStyle.borderColor
            border.width: 1
            Rectangle{
                anchors.fill: parent
                opacity: 0.15
                Image{
                    anchors.fill: parent
                    source: "Image/TalkboxBG.png"
                }
            }
            Text{
                color: defaultStyle.fontColorGold
                font.family: defaultStyle.fontFamily
                font.pointSize: defaultStyle.fontSize
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                anchors.fill: parent
                text: modelData.name
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    areaPlan = modelData
                    areaPlanLayerListView.visible = true
                }
            }
        }
    }
    DateTimeView {
        id:selectDataTime
        x: margin
        y: margin + menuHeight
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
