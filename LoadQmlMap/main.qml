import QtQuick 2.12
import QtQuick.Window 2.12
import MyItem 1.0
import "./Common"
//import "./Welcome"
//import "./Edit"
import "./Scenario"
import "./AreaPlan"

Window
{
    id: mainWindow
    visible: true
    x:0
    y:0
    //width: 1920
    //height: 1080
    title: qsTr("BIM GIS Platform v2.0.1")
    property var $app: AppGlobal{}
    property int menuHeight: defaultStyle.menuHeight
    property int menuWidth: defaultStyle.menuWidth

    OsgItem{
        type: OsgItem.Item_3DMAP
        id:showOsg
        focus: true
        anchors.fill: parent
        Component.onCompleted:{
            $app.setOsgItem(showOsg)
            $app.init()
        }
    }
    ProcessSimulationBuild
    {
        id:animationBuild
    }
    Style{
        id:defaultStyle
    }
    // -- 导航条 -------------------------------------------------------
    Rectangle{
        id:navigationBar
        width: mainWindow.width
        height: menuHeight
        color:"transparent"
        Rectangle{// 背景
            anchors.fill: parent
            opacity: 0.65
            Image{
                anchors.fill: parent
                source: "qrc:/Image/Common/TalkboxBG.png"
            }
        }
        Rectangle{// 左侧留白
            id:marginLeft
            width: (navigationBar.width - 5 * menuWidth) / 2
            color:"transparent"
        }
        MenuButton{
            id: btnWelcome
            text:qsTr("SCENARIO")
            anchors.left: marginLeft.right
            anchors.top: parent.top
            onClicked: {
                scenarioMenus.x = btnWelcome.x
                scenarioMenus.y = btnWelcome.y + btnWelcome.height
                scenarioMenus.visible = true
            }
        }
        MenuButton{
            id: btnEdit
            anchors.left: btnWelcome.right
            text:qsTr("EDIT")
            onClicked: {
                editMenus.x = btnEdit.x
                editMenus.y = btnEdit.y + btnEdit.height
                editMenus.visible = true
            }
        }
        MenuButton{
            id: btnPlay
            anchors.left: btnEdit.right
            text:qsTr("PLAY")
            onClicked: {
                playMenus.x = btnPlay.x
                playMenus.y = btnPlay.y + btnPlay.height
                playMenus.visible = true
            }
        }
        MenuButton{
            id: btnInfo
            anchors.left: btnPlay.right
            text:qsTr("INFO")
            onClicked: {
                testMsg.visible = ! testMsg.visible
            }
        }
        MenuButton{
            id: btnSystem
            anchors.left: btnInfo.right
            text:qsTr("SYSTEM")
            onClicked: {
                Qt.quit()
            }
        }
    }
    // -- 弹出菜单 -----------------------------------------------------
    Rectangle{
        id:playMenus
        visible: false
        width:columnPlayMenu.children.width
        height:columnPlayMenu.children.height
        Column{
            id:columnPlayMenu
            PopupMenuButton{
                text:qsTr("Animation")
                onClicked: {
                    playMenus.visible = false
                }
            }
            PopupMenuButton{
                text:qsTr("Simulation")
                onClicked: {
                    playMenus.visible = false

                    animationBuild.start()
                    console.log("Simulation")
                }
            }
        }
    }
    Rectangle{
        id:editMenus
        visible: false
        width:columnEditMenu.children.width
        height:columnEditMenu.children.height
        Column{
            id:columnEditMenu
            PopupMenuButton{
                text:qsTr("AreaPlan")
                onClicked: {
                    editMenus.visible = false
                    areaPlan.visible = true
                }
            }
            PopupMenuButton{
                text:qsTr("PathPlan")
                onClicked: {
                    editMenus.visible = false
                    areaPlan.visible = false
                }
            }
            PopupMenuButton{
                text:qsTr("AnimationEdit")
                onClicked: {
                    editMenus.visible = false
                }
            }
        }
    }
    Rectangle{
        id:scenarioMenus
        visible: false
        width:columnScenarioMenu.children.width
        height:columnScenarioMenu.children.height
        Column{
            id:columnScenarioMenu
            PopupMenuButton{
                text:qsTr("Scenario")
                onClicked: {
                    scenarioMenus.visible = false
                    scenarioOverView.visible = true
                }
            }
            PopupMenuButton{
                text:qsTr("Tutorials")
                onClicked: {
                    scenarioMenus.visible = false
                    scenarioOverView.visible = false
                }
            }
            PopupMenuButton{
                text:qsTr("Examples")
                onClicked: {
                    scenarioMenus.visible = false
                }
            }
        }
    }
    // -- 方案列表 -----------------------------------------
    ScenarioOverView {
        id: scenarioOverView
        y:menuHeight
        margin:defaultStyle.margin
        width: parent.width
        height: parent.height - menuHeight
        visible: false
    }
    // -- 区域编辑-----------------------------------------
    AreaPlan {
        id: areaPlan
        y:menuHeight
        margin:defaultStyle.margin
        width: parent.width
        height: parent.height - menuHeight
        visible: false
    }
    // -- 欢迎页 -------------------------------------------------------
    //Welcome {
    //    id: welcome
    //    visible: false
    //}
    //Edit {
    //    id: edit
    //    visible: false
    //}
    // -- 测试消息框 ---------------------------------------------------
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
