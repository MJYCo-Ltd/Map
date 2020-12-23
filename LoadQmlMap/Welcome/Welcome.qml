import QtQuick 2.12
import QtQuick.Controls 2.15
import "../Common"
import "../Scenario"

Rectangle {
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
        opacity: 0.15
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
        x:margin
        y:margin
        spacing:margin
        width: buttonWidth + margin * 2
        Button {
            width: buttonWidth
            font.family: defaultStyle.fontFamilyCN
            font.pointSize: defaultStyle.fontSize
            text: "Scenarios"
            checkable:true
            checked:false
            onToggled: {
                scenarioOverView.visible = true
                tutorials.visible = false
            }
        }
        Button {
            width: buttonWidth
            font.family: defaultStyle.fontFamilyCN
            font.pointSize: defaultStyle.fontSize
            text: "Tutorials"
            checkable:true
            checked:false
            onToggled: {
                tutorials.visible = true
                scenarioOverView.visible = false
            }
        }
        Button {
            width: buttonWidth
            font.family: defaultStyle.fontFamilyCN
            font.pointSize: defaultStyle.fontSize
            text: "Examples"
            checkable:true
            checked:false
        }
    }
    Rectangle{
        id:welcomeSplitLine
        anchors.left: column.right
        width:1
        height:parent.height
        color:Qt.rgba(1,1,1,1)
    }
    ScenarioOverView {
        id: scenarioOverView
        anchors.left: welcomeSplitLine.right
        margin:defaultStyle.margin
        width: parent.width - column.width - welcomeSplitLine.width
        height: parent.height
        visible: false
    }
    Rectangle {
        id: tutorials
        anchors.left: welcomeSplitLine.right
        width: parent.width - column.width - welcomeSplitLine.width
        height: parent.height
        visible: false
        color:"transparent"
    }
}
