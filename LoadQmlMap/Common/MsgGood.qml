import QtQuick 2.0
import QtGraphicalEffects 1.0

BorderMsg{
    property string text: "Good:"

    borderColor : defaultStyle.borderColorMsgGood
    bgColor : defaultStyle.bgColorMsgGood
    bgColorLight : defaultStyle.bgColorMsgGoodLight
    property int spacing : 9
    property int margin : radius * 2
    property int iconSize : 24
    height:48
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
            x:(iconSize - width) / 2
            y:(iconSize - height) / 2
            width:parent.width
            height:parent.height
            onPaint: {
                var radius = width / 3
                context.lineWidth = 2;
                context.fillStyle = parent.parent.borderColor;
                context.strokeStyle = parent.parent.borderColor;
                context.lineJoin = "round"
                context.beginPath();
                context.moveTo(width * 0.3, height * 0.5);
                context.lineTo(width * 0.4, height * 0.5);
                context.lineTo(width * 0.4, height * 0.7);
                context.lineTo(width * 0.3, height * 0.7);
                context.closePath();
                //context.arc(width / 2, height / 2, radius, 0, 2 * Math.PI);
                context.stroke();
                context.fill();

                context.lineWidth = 1.5;
                context.beginPath();
                context.moveTo(width * 0.5, height * 0.5);
                context.lineTo(width * 0.60, height * 0.34);
                context.lineTo(width * 0.61, height * 0.34);
                context.lineTo(width * 0.62, height * 0.34);
                context.lineTo(width * 0.62, height * 0.35);
                context.lineTo(width * 0.63, height * 0.38);
                context.lineTo(width * 0.56, height * 0.5);
                context.lineTo(width * 0.63, height * 0.52);
                context.lineTo(width * 0.61, height * 0.54);
                context.lineTo(width * 0.63, height * 0.6);
                context.lineTo(width * 0.61, height * 0.64);
                context.lineTo(width * 0.63, height * 0.68);
                context.lineTo(width * 0.61, height * 0.7);
                context.lineTo(width * 0.5, height * 0.7);
                context.closePath();
                context.stroke();
                context.fill();

                context.fillStyle = parent.parent.bgColor;
                context.beginPath();
                context.arc(width * 0.35, height * 0.65 , 1, 0, 2 * Math.PI);
                context.fill();
            }
        }
    }
    SequentialAnimation
    {
        loops:Animation.Infinite
        running: true
        NumberAnimation {
            target: icon;
            properties: "y"
            from: 0
            to: (height - icon.height)/2
            duration: 1000; easing.type: Easing.OutBounce
        }
        NumberAnimation {
            target: icon;
            properties: "y"
            from: (height - icon.height)/2
            to: 0
            duration: 1000; //easing.type: Easing.OutBounce
        }
    }
    Text{
        x : parent.margin + iconSize + parent.spacing
        y: (parent.height - height) / 2
        font.family: "Helvetica"
        font.pointSize: 13
        color: defaultStyle.fontColorMsgGood
        text:parent.text
    }
}
