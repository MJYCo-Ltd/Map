import QtQuick 2.0
import QtGraphicalEffects 1.0

BorderMsg{
    property string text: "Done:"

    borderColor : defaultStyle.borderColorMsgDone
    bgColor : defaultStyle.bgColorMsgDone
    bgColorLight : defaultStyle.bgColorMsgDoneLight
    property int spacing : 9
    property int margin : radius * 2
    property int iconSize : 24
    Rectangle{
        id:icon
        property int size: 20
        x:margin
        y:(parent.height - height)/2
        width:size
        height: size
        color:"transparent"
        rotation:0
        Canvas{
            id: backgroundCanvas
            contextType: "2d"
            x:(iconSize - width) / 2
            y:(iconSize - height) / 2
            width:parent.width
            height:parent.height
            onPaint: {
                var radius = width / 3
                context.lineWidth = 1.5;
                context.strokeStyle = parent.parent.borderColor;
                context.beginPath();
                context.arc(width / 2, height / 2, radius, 0, 2 * Math.PI);
                context.stroke();

                context.lineJoin = "round"
                context.strokeStyle = parent.parent.borderColor;
                context.beginPath();
                context.moveTo(width * 7 / 24, height / 2);
                context.lineTo(width * 11 / 24, height * 2 / 3);
                context.lineTo(width * 17 / 24, height * 9 / 24);
                context.stroke();

            }
        }
        SequentialAnimation{
            loops:Animation.Infinite
            running: true
            PropertyAnimation{
                target: icon
                properties: "size"
                from: 20
                to: 24
                duration: 600
            }
            PropertyAnimation{
                target: icon
                properties: "size"
                from: 24
                to: 20
                duration: 600
            }
            RotationAnimation{
                target: icon
                from: 0
                to: 15
                duration: 100
            }
            RotationAnimation{
                target: icon
                from: 15
                to: 0
                duration: 100
            }
            RotationAnimation{
                target: icon
                from: 0
                to: -15
                duration: 100
            }
            RotationAnimation{
                target: icon
                from: -15
                to: 0
                duration: 100
            }
        }
    }
    Text{
        x : parent.margin + iconSize + parent.spacing
        y: (parent.height - height) / 2
        font.family: "Helvetica"
        font.pointSize: 13
        color: defaultStyle.fontColorMsgDone
        text:parent.text
    }
}
