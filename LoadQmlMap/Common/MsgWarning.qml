import QtQuick 2.0
import QtGraphicalEffects 1.0

BorderMsg{
    property string text: "Warning:"

    borderColor : defaultStyle.borderColorMsgWarning
    bgColor : defaultStyle.bgColorMsgWarning
    bgColorLight : defaultStyle.bgColorMsgWarningLight
    property int spacing : 9
    property int margin : radius * 2
    property int iconSize : 24
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
                context.lineJoin = "round"
                context.fillStyle = parent.parent.borderColor;
                context.strokeStyle = parent.parent.borderColor;
                context.beginPath();
                context.moveTo(width / 2, height / 6);
                context.lineTo(width * (0.5 - 1.73 / 6), height * 2.0 / 3);
                context.lineTo(width * (0.5 + 1.73 / 6), height * 2.0 / 3);
                context.closePath();
                context.fill();
                context.lineWidth = 3;
                context.stroke();

                context.lineWidth = 1.5;
                context.strokeStyle = "black";
                context.beginPath();
                context.moveTo(width / 2, height * 7 / 24);
                context.lineTo(width / 2, height * 12 / 24);
                context.moveTo(width / 2, height * 13 / 24);
                context.lineTo(width / 2, height * 15 / 24);
                context.stroke();

            }
        }
        SequentialAnimation{
            loops:Animation.Infinite
            running: true
            PropertyAnimation{
                target: icon
                properties: "opacity"
                from: 1
                to: 0
                duration: 600
            }
            PropertyAnimation{
                target: icon
                properties: "opacity"
                from: 0
                to: 1
                duration: 600
            }
        }
    }
    Text{
        x : parent.margin + iconSize + parent.spacing
        y: (parent.height - height) / 2
        font.family: defaultStyle.fontFamilyEN
        font.pointSize: 13
        color: defaultStyle.fontColorMsgWarning
        text:parent.text
    }
}
