import QtQuick 2.15
import QtGraphicalEffects 1.0

Rectangle {
    id:borderImg3
    width:200
    height:48
    property int corner: width > height ? (height / 5) : (width / 5)
    property int radius: corner > 20 ? 20 : corner
    color: "transparent"
    opacity : 0.1
    Rectangle
    {
        id:background
        anchors.fill: parent
        Image{
            anchors.fill: parent
            source: "Image/TalkboxBG.png"
        }
    }
    PropertyAnimation{
        id:enterAnim
        target:borderImg3
    }
    MouseArea{
       anchors.fill:parent
       hoverEnabled: true
       onEntered:  {
           parent.opacity = 1
       }
       onExited: PropertyAnimation {
           target: borderImg3;
           property: "opacity";
           to: 0.1;
           duration: 1000
       }
    }
    /*
    Canvas {
        width: parent.width
        height: parent.height
        contextType: "2d"
        onPaint: {
            context.lineWidth = 2
            context.strokeStyle = defaultStyle.primaryColor
            context.beginPath();
            context.moveTo(0, 0)
            context.lineTo(0, height)
            context.lineTo(width, height);
            context.lineTo(width, 0);
            context.closePath();
            context.stroke();

            context.strokeStyle = defaultStyle.borderColor//defaultStyle.primaryColor
            context.lineWidth = 3
            context.beginPath();
            context.moveTo(radius, 0)
            context.lineTo(0, 0)
            context.lineTo(0, radius)

            context.moveTo(0, height - radius);
            context.lineTo(0, height);
            context.lineTo(radius, height);

            context.moveTo(width - radius, height);
            context.lineTo(width, height);
            context.lineTo(width, height - radius);

            context.moveTo(width - radius, 0);
            context.lineTo(width, 0);
            context.lineTo(width, radius);

            context.stroke();
        }
    }*/
}
