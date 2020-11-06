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
    ScenarioManager{
        id:scenarioManager
    }
    AreaPlanManager{
        id:areaPlanManager
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

    ButtonA{
        id: btnScenario
        width: 128
        height: menuHeight
        border.width: 0
        BorderImg2
        {
            color:"transparent"
            secondaryColor: Qt.rgba(250,250,255,250)
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
        id: btnDataTime
        anchors.left: btnScenario.right
        width: 128
        height: menuHeight
        border.width: 0
        BorderImg2
        {
            color:"transparent"
            secondaryColor: Qt.rgba(250,250,255,250)
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
    ButtonA{
        id: btnMsgBox
        anchors.left: btnDataTime.right
        width: 128
        height: menuHeight
        border.width: 0
        BorderImg2
        {
            color:"transparent"
            secondaryColor: Qt.rgba(250,250,255,250)
            titleWidth: 0
            titleHeight: 0
            opacity:  0.2
            margin : 3
            anchors.fill: parent
        }
        text:qsTr("消息框")
        onClicked: {
            testMsg.visible = ! testMsg.visible
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
        x: 0
        y: menuHeight
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
