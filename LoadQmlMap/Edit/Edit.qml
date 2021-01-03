import QtQuick 2.12
import QtQuick.Controls 2.15
import "../Common"
import "../Animation"

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
    ButtonGroup {
        buttons: column.children
        onClicked: console.log("clicked:", button.text)
    }
    Column {
        id: column
        y:margin
        spacing:margin
        width: buttonWidth + margin * 2
        Button {
            x:margin
            width: buttonWidth
            font.family: defaultStyle.fontFamilyCN
            font.pointSize: defaultStyle.fontSize
            text: "AreaPlan"
            checkable:true
            checked:false
            onToggled: {
                pathplan.visible = true
                animation.visible = false
            }
        }
        Button {
            x:margin
            width: buttonWidth
            font.family: defaultStyle.fontFamilyCN
            font.pointSize: defaultStyle.fontSize
            text: "PathPlan"
            checkable:true
            checked:false
            onToggled: {
                pathplan.visible = true
                animation.visible = false
            }
        }
        Button {
            x:margin
            width: buttonWidth
            font.family: defaultStyle.fontFamilyCN
            font.pointSize: defaultStyle.fontSize
            text: "Animation"
            checkable:true
            checked:false
            onToggled: {
                animation.visible = true
                pathplan.visible = false
            }
        }
    }
    Rectangle{
        id:editSplitLine
        anchors.left: column.right
        width:1
        height:edit.height
        color:Qt.rgba(1,1,1,1)
    }
    Animation {
        id: animation
        anchors.left: editSplitLine.right
        margin:defaultStyle.margin
        width: edit.width - column.width - editSplitLine.width
        height: edit.height
        visible: false
    }
    Rectangle {
        id: pathplan
        anchors.left: editSplitLine.right
        width: edit.width - column.width - editSplitLine.width
        height: edit.height
        visible: false
        color:"transparent"
    }
}
