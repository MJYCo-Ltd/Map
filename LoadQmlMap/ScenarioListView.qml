import QtQuick 2.0

Rectangle
{
    width : 400
    height: parent.height
    color: defaultStyle.backgroundColor
    property int margin: defaultStyle.margin
    BorderImg{
        id: borderScenario
        anchors.fill: parent
        margin: parent.margin
    }
    ListView {
        x: margin * 2
        y: margin * 2
        width: parent.width - margin * 4
        height: parent.height - margin * 4
        model: scenarioManager.scenarios
        delegate: Rectangle {
            width: parent.width
            height: 40
            color: "transparent"
            Rectangle {
                y:parent.height - 1
                width:parent.width
                height:1
                color:defaultStyle.fontColor
            }
            Text {
                color:defaultStyle.fontColor
                //anchors.left: parent.left
                anchors.centerIn: parent
                text: modelData
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    Qt.quit()
                }
            }
        }
    }
}

