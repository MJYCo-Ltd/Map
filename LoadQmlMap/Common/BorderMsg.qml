import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0

Rectangle {
    property color borderColor : defaultStyle.borderColorMsgInfo
    property color bgColor : defaultStyle.bgColorMsgInfo
    property color bgColorLight : defaultStyle.bgColorMsgInfoLight
    property int borderRadius : 6
    opacity: 0.75
    width:600
    height:48
    color:bgColor
    radius: borderRadius
    border{
        color: borderColor
        width: 1
    }
    ColorAnimation on color {
        id:enterAnim
        to: bgColorLight
        duration: 200
    }
    ColorAnimation on color {
        id:exitAnim
        to: bgColor
        duration: 200
    }
    MouseArea{
        anchors.fill:parent
        hoverEnabled: true
        onEntered: {
            enterAnim.start()            
            opacity = 1
        }
        onExited: {
            exitAnim.start()            
            opacity = 0.75
        }
     }

    Canvas {
        id: backgroundCanvas
        contextType: "2d"
        x:0
        y:0
        width:parent.width
        height:parent.height
        onPaint: {
            var radius = borderRadius / 2
            context.lineWidth = 2;
            context.fillStyle = parent.borderColor;
            context.beginPath();
            context.moveTo(0, height / 3 - radius);
            context.lineTo(radius, height / 3);
            context.lineTo(radius, height * 2 / 3);
            context.lineTo(0, height * 2/ 3 + radius);
            context.lineTo(0, height / 3 - radius);
            context.closePath();
            context.fill();

            context.beginPath();
            context.strokeStyle = parent.borderColor
            context.arc(0, height / 3, radius,  0.5 * Math.PI, 0)
            context.arc(0, height * 2/ 3, radius, 0, 0.5 * Math.PI)
            context.stroke();
        }
    }
    Glow {
        anchors.fill: backgroundCanvas
        radius: 2
        samples: 37
        color: "#87CEFA"
        source: backgroundCanvas
    }

}
