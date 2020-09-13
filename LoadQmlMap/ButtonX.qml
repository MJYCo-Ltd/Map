import QtQuick 2.2
import QtQuick.Controls 1.2

Rectangle {
    id:buttonX
    width:40
    height:40
    color: "transparent"
    //color:defaultStyle.backgroundColor
    property color borderColor : defaultStyle.borderColor
    //border.color: defaultStyle.borderColor
    property string text:"x"
    signal clicked(var string);
    BorderImg3
    {
        anchors.fill:parent
    }
    MouseArea{
        anchors.fill:parent
        onClicked: {
            buttonX.clicked(buttonX.text)
            console.log(buttonX.text)
        }
        hoverEnabled: true
        onEntered: {
            color = defaultStyle.bgColorMsgAlertLight
            borderColor = defaultStyle.borderColorMsgAlert
            backgroundCanvas.requestPaint()

        }
        onExited: {
            color = defaultStyle.backgroundColor
            borderColor = defaultStyle.borderColor
            backgroundCanvas.requestPaint()
        }
    }
    Canvas{
        id: backgroundCanvas
        contextType: "2d"
        width:parent.width
        height:parent.height
        onPaint: {
            context.lineWidth = 1.5;
            context.strokeStyle = borderColor;
            context.beginPath();
            context.moveTo(width * 9 / 24, height * 9 / 24);
            context.lineTo(width * 15 / 24, height * 15 / 24);
            context.moveTo(width * 9 / 24, height * 15 / 24);
            context.lineTo(width * 15 / 24, height * 9 / 24);
            context.stroke();
        }
    }
}
