/**
 * 点击变色按钮
 * 添加到其他组建中不触发enter,exit
 */
import QtQuick 2.2
import QtQuick.Controls 1.2

Rectangle {
    id:root
    width: 100
    height: 28
    color: "transparent"
    state: "RELEASE"
    border.color: "#B0C4DE"

    signal clicked(var string);

    property string text:"button"
    property color pressColor: "#969ff0de"
    property color releaseColor: "transparent"
    property int fontSize: 12

    Text {
        id:btnText
        color: defaultStyle.fontColor
        text: root.text
        font.family: defaultStyle.fontFamily
        font.pointSize: fontSize
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.fill: parent
    }

    MouseArea {
        id: mouseAreaBtn
        anchors.fill: parent
        onClicked: {
            root.clicked(btnText.text)
        }
    }
    states:[
        State{
            name:"RELEASE"
            when: !mouseAreaBtn.pressed
            PropertyChanges {
                target: root
                color:root.releaseColor
            }
        },
        State{
            name:"PRESSED"
            when: mouseAreaBtn.pressed
            PropertyChanges {
                target: root
                color:root.pressColor
            }
        }

    ]
    transitions:
        [
        Transition {
            from: "RELEASE"
            to: "PRESSED"
            ColorAnimation {
                duration: 1000
            }
        },
        Transition {
            from: "PRESSED"
            to: "RELEASE"
            ColorAnimation {
                duration: 1000
            }
        }
    ]
}
