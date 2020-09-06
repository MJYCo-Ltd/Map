import QtQuick 2.15
import QtGraphicalEffects 1.0

Rectangle {
    property int margin: 10
    property int radius: parent.margin / 2
    property int spacing: defaultStyle.lineWidth + 2
    color:Qt.rgba(17/255,17/255,53/255, 5/255)
    Rectangle
    {
        id:borderBackground
        x:parent.margin
        y:parent.margin
        width: parent.width - 2 * parent.margin
        height: parent.height - 2 * parent.margin
        color:Qt.rgba(17/255,17/255,53/255, 55/255)
    }
    Rectangle
    {
        id:bottomBar
        x:parent.margin * 3
        y:parent.height - parent.margin - defaultStyle.lineWidth
        width: parent.width - 4 * parent.margin
        height: defaultStyle.lineWidth
        gradient: Gradient{
            GradientStop{position:0.0; color:defaultStyle.primaryColor}
            GradientStop{position:0.6; color:defaultStyle.secondaryColor}
            GradientStop{position:1.0; color:defaultStyle.primaryColor}
        }
    }
    Rectangle
    {
        id:leftBottomSignal
        x:parent.margin + defaultStyle.lineWidth
        y:parent.height - parent.margin - defaultStyle.lineWidth - parent.radius * 2
        width: parent.radius * 2
        height: parent.radius * 2
        color:"transparent"
        Canvas {
            x : 0
            y : 0
            width: parent.width
            height: parent.height
            contextType: "2d"
            onPaint: {
                context.lineWidth = 1
                context.strokeStyle = defaultStyle.secondaryColor
                context.fillStyle = defaultStyle.primaryColor
                context.beginPath();
                context.moveTo(width * 0.1, height * 0.8)
                context.lineTo(width * 0.2, height * 0.9)
                context.lineTo(width * 0.32,  height * 0.68);
                context.closePath();
                context.fill()
                context.stroke();

                var radius_max = parent.width / 2
                var radius_iner = radius_max * 0.3
                var radius_outer = radius_max * 1.3
                var r = radius_iner;
                while(r < radius_outer)
                {
                    var angleStart = 0.025 * Math.PI;
                    var angleEnd = - 0.525 * Math.PI;
                    context.beginPath();
                    context.arc(parent.width * 0.225 , parent.height * 0.725, r, angleStart, angleEnd, true);
                    context.stroke();

                    r = r + radius_max * 0.2;
                }
            }
        }
        Glow {
            anchors.fill: leftBottomSignal
            radius: parent.spacing * 2
            samples: 37
            color: "#87CEFA"
            source: leftBottomSignal
        }
    }
    Rectangle
    {
        id:rightBottomSignal
        x:parent.width - parent.margin - defaultStyle.lineWidth - parent.radius * 2
        y:parent.height - parent.margin - defaultStyle.lineWidth - parent.radius * 2
        width: parent.radius * 2
        height: parent.radius * 2
        color:"transparent"
        Canvas {
            x : 0
            y : 0
            width: parent.width
            height: parent.height
            contextType: "2d"
            onPaint: {
                context.lineWidth = 1
                context.strokeStyle = defaultStyle.secondaryColor
                context.fillStyle = defaultStyle.primaryColor
                context.beginPath();
                context.moveTo(width * 0.8, height * 0.9)
                context.lineTo(width * 0.9, height * 0.8)
                context.lineTo(width * 0.68,  height * 0.68);
                context.closePath();
                context.fill()
                context.stroke();

                var radius_max = parent.width / 2
                var radius_iner = radius_max * 0.3
                var radius_outer = radius_max * 1.3
                var r = radius_iner;
                while(r < radius_outer)
                {
                    var angleStart = - 0.475 * Math.PI;
                    var angleEnd = - 1.025 * Math.PI;
                    context.beginPath();
                    context.arc(parent.width * 0.725 , parent.height * 0.725, r, angleStart, angleEnd, true);
                    context.stroke();

                    r = r + radius_max * 0.2;
                }
            }
        }
        Glow {
            anchors.fill: rightBottomSignal
            radius: parent.spacing * 2
            samples: 37
            color: "#87CEFA"
            source: rightBottomSignal
        }
    }
    Rectangle
    {
        id: leftBorder
        x:parent.margin
        y:parent.margin + parent.radius
        width: defaultStyle.lineWidth
        height: parent.height - parent.radius - 2 * parent.margin
        gradient: Gradient{
            GradientStop{position:0.0; color:defaultStyle.primaryColor}
            GradientStop{position:0.6; color:defaultStyle.secondaryColor}
            GradientStop{position:1.0; color:defaultStyle.primaryColor}
        }
    }
    Rectangle
    {
        id: leftBorder3
        x:parent.margin + defaultStyle.lineWidth
        y:parent.margin + parent.radius
        width: defaultStyle.lineWidth
        height: (parent.height - 2 * parent.margin) * 1 / 3
        gradient: Gradient{
            GradientStop{position:0.0; color:defaultStyle.primaryColor}
            GradientStop{position:0.2; color:defaultStyle.secondaryColor}
            GradientStop{position:1.0; color:defaultStyle.primaryColor}
        }
    }
    Rectangle
    {
        id: rightBorder
        x: parent.width - parent.margin
        y: parent.margin
        width: defaultStyle.lineWidth
        height: parent.height - 2 * parent.margin - 2 * defaultStyle.lineWidth
        gradient: Gradient{
            GradientStop{position:0.0; color:defaultStyle.primaryColor}
            GradientStop{position:0.6; color:defaultStyle.secondaryColor}
            GradientStop{position:1.0; color:defaultStyle.primaryColor}
        }
    }
    Rectangle
    {
        id: rightBorder2
        x: parent.width - parent.margin - parent.spacing / 2
        y: parent.margin + (parent.height - 2 * parent.margin) * 1 / 6
        width: defaultStyle.lineWidth
        height: (parent.height - 2 * parent.margin) * 5 / 6 - defaultStyle.lineWidth
        gradient: Gradient{
            GradientStop{position:0.0; color:defaultStyle.primaryColor}
            GradientStop{position:0.4; color:defaultStyle.secondaryColor}
            GradientStop{position:1.0; color:defaultStyle.primaryColor}
        }
    }
    Rectangle
    {
        id: rightBorder3
        x: parent.width - parent.margin - parent.spacing
        y: parent.margin + (parent.height - 2 * parent.margin) * 1 / 3
        width: defaultStyle.lineWidth
        height: (parent.height - 2 * parent.margin) * 2 / 3
        gradient: Gradient{
            GradientStop{position:0.0; color:defaultStyle.primaryColor}
            GradientStop{position:0.2; color:defaultStyle.secondaryColor}
            GradientStop{position:1.0; color:defaultStyle.primaryColor}
        }
    }
    Rectangle
    {
        id: topBorder
        x:parent.radius + parent.margin + parent.spacing
        y:parent.margin
        width: parent.width - 2 * parent.margin - parent.radius - parent.spacing
        height: defaultStyle.lineWidth
        gradient: Gradient{
            GradientStop{position:0.0; color:defaultStyle.primaryColor}
            GradientStop{position:0.6; color:defaultStyle.secondaryColor}
            GradientStop{position:1.0; color:defaultStyle.primaryColor}
        }
    }
    Rectangle
    {
        id: bottomBorder
        x:parent.margin
        y:parent.height - parent.margin
        width: parent.width - 2 * parent.margin - parent.spacing - 2 * defaultStyle.lineWidth
        height: defaultStyle.lineWidth
        gradient: Gradient{
            GradientStop{position:0.0; color:defaultStyle.primaryColor}
            GradientStop{position:0.6; color:defaultStyle.secondaryColor}
            GradientStop{position:1.0; color:defaultStyle.primaryColor}
        }
    }
    Rectangle
    {
        id: leftTop
        x:parent.margin
        y:parent.margin
        width: parent.radius * 1.414
        height: parent.radius
        color:"transparent"
        Rectangle
        {
            id: leftTopRot
            x:0
            y:leftTop.height
            width: parent.width
            height: defaultStyle.lineWidth
            gradient: Gradient{
                GradientStop{position:0.0; color:defaultStyle.primaryColor}
                GradientStop{position:0.6; color:defaultStyle.secondaryColor}
                GradientStop{position:1.0; color:defaultStyle.primaryColor}
            }
            Glow {
                anchors.fill: leftTopRot
                radius: parent.spacing * 3
                samples: 37
                color: "#87CEFA"
                source: leftTopRot
            }
            transform: Rotation
            {
                origin.x: 0;
                origin.y: leftTopRot.height;
                axis { x: 0; y: 0; z: 1 }
                angle: -45
            }
        }
    }
    Rectangle
    {
        id: leftTop2
        x:parent.margin
        y:parent.margin
        width: parent.radius * 1.414
        height: parent.radius
        color:"transparent"

        Rectangle
        {
            id: leftTop2Rot
            x: defaultStyle.lineWidth * 2
            y: leftTop.height - defaultStyle.lineWidth
            width: parent.width
            height: defaultStyle.lineWidth
            gradient: Gradient{
                GradientStop{position:0.0; color:defaultStyle.primaryColor}
                GradientStop{position:0.6; color:defaultStyle.secondaryColor}
                GradientStop{position:1.0; color:defaultStyle.primaryColor}
            }
            Glow {
                anchors.fill: leftTop2Rot
                radius: parent.spacing * 3
                samples: 37
                color: "#87CEFA"
                source: leftTop2Rot
            }
            transform: Rotation
            {
                origin.x: defaultStyle.lineWidth;
                origin.y: leftTop2Rot.height;
                axis { x: 0; y: 0; z: 1 }
                angle: -45
            }
        }
    }
    Glow {
        anchors.fill: leftBorder3
        radius: parent.spacing * 1
        samples: 37
        color: "#87CEFA"
        source: leftBorder3
    }
    Glow {
        anchors.fill: leftBorder
        radius: parent.spacing * 3
        samples: 37
        color: "#87CEFA"
        source: leftBorder
    }
    Glow {
        anchors.fill: rightBorder3
        radius: parent.spacing * 1
        samples: 37
        color: "#87CEFA"
        source: rightBorder3
    }
    Glow {
        anchors.fill: rightBorder2
        radius: parent.spacing * 2
        samples: 37
        color: "#87CEFA"
        source: rightBorder2
    }
    Glow {
        anchors.fill: rightBorder
        radius: parent.spacing * 3
        samples: 37
        color: "#87CEFA"
        source: rightBorder
    }
    Glow {
        anchors.fill: topBorder
        radius: parent.spacing * 3
        samples: 37
        color: "#87CEFA"
        source: topBorder
    }
    Glow {
        anchors.fill: bottomBorder
        radius: parent.spacing * 3
        samples: 37
        color: "#87CEFA"
        source: bottomBorder
    }
}
