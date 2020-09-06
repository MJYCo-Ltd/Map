import QtQuick 2.0

Rectangle
{
    width : 400
    height: parent.height
    color:"transparent"
    property int margin: defaultStyle.margin
    BorderImg{
        id: borderScenario
        anchors.fill: parent
        margin: parent.margin
    }
    ListView {
        x : 2 * borderScenario.margin
        y : 2 * borderScenario.margin
        width : parent.width - 4 * borderScenario.margin
        height : parent.height - 4 * borderScenario.margin
        model: scenarioManager.scenarios
        delegate: Rectangle {
            width: parent.width
            height: 40
            color:"transparent"
            border.color:"transparent"

            Text {
                color:defaultStyle.fontColor
                anchors.left: parent.left
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

