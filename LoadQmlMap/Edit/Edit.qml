import QtQuick 2.12
import QtQuick.Controls 2.15
import "../Common"
import "../Animation"
import "../AreaPlan"

Rectangle {
    id:edit
    property int buttonWidth : 160
    property int buttonHeight : 42
    property int margin: defaultStyle.margin
    property int spacing : margin

    x: margin
    y: margin + menuHeight
    width:parent.width - margin * 2
    height:parent.height - menuHeight - margin * 2
    color:"transparent"
    Rectangle{
        anchors.fill: parent
        opacity: 0.65
        Image{
            anchors.fill: parent
            source: "qrc:/Image/Common/TalkboxBG.png"
        }
    }
    // -- 按钮 ------------------------------------------
    Column{
        id: column
        x: margin
        y: margin
        width:buttonWidth + margin
        height:edit.height

        ButtonGroup {
            buttons: columnButtons.children
            //onClicked: console.log("clicked:", button.text)
        }
        Column {
            id: columnButtons
            spacing:margin
            Button {
                width: buttonWidth
                height:buttonHeight
                font.family: defaultStyle.fontFamilyCN
                font.pointSize: defaultStyle.fontSize
                text: "AreaPlan"
                checkable:true
                checked:false
                onToggled: {
                    areaPlan.visible = true
                    pathplan.visible = false
                    animation.visible = false
                }
            }
            Button {
                width: buttonWidth
                height:buttonHeight
                font.family: defaultStyle.fontFamilyCN
                font.pointSize: defaultStyle.fontSize
                text: "PathPlan"
                checkable:true
                checked:false
                onToggled: {
                    pathplan.visible = true
                    areaPlan.visible = false
                    animation.visible = false
                }
            }
            Button {
                width: buttonWidth
                height:buttonHeight
                font.family: defaultStyle.fontFamilyCN
                font.pointSize: defaultStyle.fontSize
                text: "Animation"
                checkable:true
                checked:false
                onToggled: {
                    animation.visible = true
                    areaPlan.visible = false
                    pathplan.visible = false
                }
            }
        }
        Rectangle
        {
            width: buttonWidth
            height: edit.height - columnButtons.height - buttonSave.height - margin * 2
            color: "transparent"
        }
        Button {
            id:buttonSave
            width: buttonWidth
            height:buttonHeight
            font.family: defaultStyle.fontFamilyCN
            font.pointSize: defaultStyle.fontSize
            text: qsTr("Save")
            onClicked: {
                $app.scenarioManager().save()
            }
        }
    }

    // -- 分割线 ----------------------------------------
    Rectangle{
        id:editSplitLine
        anchors.left: column.right
        width:1
        height:edit.height
        color:Qt.rgba(1,1,1,1)
    }
    // -- 区域编辑-----------------------------------------
    AreaPlan {
        id: areaPlan
        anchors.left: editSplitLine.right
        margin:defaultStyle.margin
        width: edit.width - column.width - editSplitLine.width
        height: edit.height
        visible: false
    }
    // -- 动画编辑-----------------------------------------
    Animation {
        id: animation
        anchors.left: editSplitLine.right
        margin:defaultStyle.margin
        width: edit.width - column.width - editSplitLine.width
        height: edit.height
        visible: false
    }
    // -- 路径编辑-----------------------------------------
    Rectangle {
        id: pathplan
        anchors.left: editSplitLine.right
        width: edit.width - column.width - editSplitLine.width
        height: edit.height
        visible: false
        color:"transparent"
    }
}
