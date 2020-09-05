import QtQuick 2.0

ListView {
    width: 200
    height: parent.height
    opacity:0.5
    model: scenarioManager.scenarios
    delegate: Rectangle {
        width: parent.width
        height: 40
        border.color: Qt.darker(color)

        Text {
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
