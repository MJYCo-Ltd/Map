import QtQuick 2.0
import QtGraphicalEffects 1.0

BorderMsg{
    property string text: "Info:"

    borderColor : defaultStyle.borderColorMsgInfo
    bgColor : defaultStyle.bgColorMsgInfo
    bgColorLight : defaultStyle.bgColorMsgInfoLight
    property int spacing : 9
    property int margin : radius * 2
    property int iconSize : 22
    Rectangle{
        id:icon
        x:margin
        y:(parent.height - height)/2
        width:iconSize
        height: iconSize
        color:"transparent"
        rotation:0
        Canvas{
            id: backgroundCanvas
            contextType: "2d"
            x:0
            y:0
            width:parent.width
            height:parent.height
            onPaint: {
                var radius = width / 3
                context.fillStyle = parent.parent.borderColor;
                context.beginPath();
                context.arc(width / 2, height / 2, radius, 0, 2 * Math.PI);
                context.fill();

                context.fillStyle = parent.parent.bgColor;
                context.beginPath();
                context.arc(width / 2, height / 3, 1.5, 0, 2 * Math.PI);
                context.fill();

                context.strokeStyle = parent.parent.bgColor;
                context.lineWidth = 2.5;
                context.beginPath();
                //context.moveTo(parent.width * 0.497, parent.height * 0.45);
                context.moveTo(parent.width * 0.498, parent.height * 0.45);
                context.lineTo(parent.width * 0.498, parent.height * 0.65);
                //context.lineTo(parent.width * 0.503, parent.height * 0.7);
                context.lineTo(parent.width * 0.502, parent.height * 0.65);
                context.lineTo(parent.width * 0.502, parent.height * 0.45);
                //context.closePath();
                context.stroke();
                //context.fill();
            }
        }
        SequentialAnimation{
            loops:Animation.Infinite
            running: true
            RotationAnimation{
                target: icon
                from: 0
                to: 25
                duration: 100
            }
            RotationAnimation{
                target: icon
                from: 25
                to: 0
                duration: 100
            }
            RotationAnimation{
                target: icon
                from: 0
                to: -25
                duration: 100
            }
            RotationAnimation{
                target: icon
                from: -25
                to: 0
                duration: 100
            }
        }
    }
    Text{
        x : parent.margin + iconSize + parent.spacing
        y: (parent.height - height) / 2
        font.family: defaultStyle.fontFamilyEN
        font.pointSize: 13
        color: defaultStyle.fontColorMsgInfo
        text:parent.text
    }
}
