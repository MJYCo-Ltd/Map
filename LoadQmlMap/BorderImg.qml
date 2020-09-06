import QtQuick 2.15
import QtGraphicalEffects 1.0

Rectangle {
    id:border
    property int margin: 10
    property int radius: border.margin * 4
    property int spacing: lineWidth + 2
    property color primaryColor: Qt.rgba(80/255,170/255,212/255,255/255)
    property color secondaryColor: Qt.rgba(30/255,84/255,136/255,255/255)
    property int  lineWidth: 2
    color:Qt.rgba(17/255,17/255,53/255, 5/255)

    //BorderImgCircle
    //{
    //    id :leftBottomCircle
    //    x: 0.8 * border.margin
    //    y:border.height - 2.8 * border.margin
    //    width: 2 * border.margin
    //    height: 2 * border.margin
    //}
    //Glow {
    //    anchors.fill: leftBottomCircle
    //    radius: border.spacing * 3
    //    samples: 37
    //    color: "#87CEFA"
    //    source: leftBottomCircle
    //}

    Rectangle
    {
        id:borderBackground
        x:border.margin
        y:border.margin
        width: parent.width - 2 * border.margin
        height: parent.height - 2 * border.margin
        color:Qt.rgba(17/255,17/255,53/255, 55/255)
    }
    Rectangle
    {
        id:bottomBar
        x:border.margin * 3
        y:border.height - border.margin - border.lineWidth
        width: parent.width - 4 * border.margin
        height: border.lineWidth
        gradient: Gradient{
            GradientStop{position:0.0; color:border.primaryColor}
            GradientStop{position:0.6; color:border.secondaryColor}
            GradientStop{position:1.0; color:border.primaryColor}
        }
    }
    Rectangle
    {
        id:leftBottomSignal
        x:border.margin + border.lineWidth
        y:border.height - border.margin - border.lineWidth - border.radius * 2
        width: border.radius * 2
        height: border.radius * 2
        color:"transparent"
        Canvas {
            x : 0
            y : 0
            width: parent.width
            height: parent.height
            contextType: "2d"
            onPaint: {
                context.lineWidth = 1
                context.strokeStyle = border.secondaryColor
                context.fillStyle = border.primaryColor
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
            radius: border.spacing * 2
            samples: 37
            color: "#87CEFA"
            source: leftBottomSignal
        }
    }Rectangle
    {
        id:rightBottomSignal
        x:border.width - border.margin - border.lineWidth - border.radius * 2
        y:border.height - border.margin - border.lineWidth - border.radius * 2
        width: border.radius * 2
        height: border.radius * 2
        color:"transparent"
        Canvas {
            x : 0
            y : 0
            width: parent.width
            height: parent.height
            contextType: "2d"
            onPaint: {
                context.lineWidth = 1
                context.strokeStyle = border.secondaryColor
                context.fillStyle = border.primaryColor
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
            radius: border.spacing * 2
            samples: 37
            color: "#87CEFA"
            source: rightBottomSignal
        }
    }
    /*
    Rectangle
    {
        id:rightBottomPolygon
        x:border.width - border.margin - border.radius
        y:border.width - border.margin - border.radius
        width: border.radius + border.lineWidth
        height: border.radius + border.lineWidth
        color:"transparent"
        Canvas {
            id: canvasRightBottomPolygon
            width: parent.width
            height: parent.height
            contextType: "2d"
            onPaint: {
                context.lineWidth = 4
                context.strokeStyle = border.primaryColor
                context.fillStyle = border.secondaryColor
                context.beginPath();
                context.moveTo(rightBottomPolygon.width * 0.8, rightBottomPolygon.height * 0.4)
                context.lineTo(rightBottomPolygon.width * 0.4, rightBottomPolygon.height * 0.8)
                context.lineTo(0, rightBottomPolygon.height);
                context.lineTo(rightBottomPolygon.width, 0);
                context.closePath();
                context.fill()
                context.stroke();
                context.clip();
            }
        }
    }*/

    Rectangle
    {
        id: leftBorder
        x:border.margin
        y:border.margin + border.radius
        width: border.lineWidth
        height: parent.height - border.radius - 2 * border.margin
        gradient: Gradient{
            GradientStop{position:0.0; color:border.primaryColor}
            GradientStop{position:0.6; color:border.secondaryColor}
            GradientStop{position:1.0; color:border.primaryColor}
        }
    }
    //Rectangle
    //{
    //    id: leftBorder2
    //    x:border.margin + border.spacing / 2
    //    y:border.margin + border.radius
    //    width: border.lineWidth
    //    height: (border.height - border.radius - 2 * border.margin) * 5 / 6
    //    gradient: Gradient{
    //        GradientStop{position:0.0; color:border.primaryColor}
    //        GradientStop{position:0.4; color:border.secondaryColor}
    //        GradientStop{position:1.0; color:border.primaryColor}
    //    }
    //}
    Rectangle
    {
        id: leftBorder3
        x:border.margin + border.lineWidth
        y:border.margin + border.radius
        width: border.lineWidth
        height: (border.height - 2 * border.margin) * 1 / 3
        gradient: Gradient{
            GradientStop{position:0.0; color:border.primaryColor}
            GradientStop{position:0.2; color:border.secondaryColor}
            GradientStop{position:1.0; color:border.primaryColor}
        }
    }
    Rectangle
    {
        id: rightBorder
        x: parent.width - border.margin
        y: border.margin
        width: border.lineWidth
        height: border.parent.height - 2 * border.margin - 2 * border.lineWidth
        gradient: Gradient{
            GradientStop{position:0.0; color:border.primaryColor}
            GradientStop{position:0.6; color:border.secondaryColor}
            GradientStop{position:1.0; color:border.primaryColor}
        }
    }
    Rectangle
    {
        id: rightBorder2
        x: parent.width - border.margin - border.spacing / 2
        y: border.margin + (border.parent.height - 2 * border.margin) * 1 / 6
        width: border.lineWidth
        height: (border.parent.height - 2 * border.margin) * 5 / 6 - border.lineWidth
        gradient: Gradient{
            GradientStop{position:0.0; color:border.primaryColor}
            GradientStop{position:0.4; color:border.secondaryColor}
            GradientStop{position:1.0; color:border.primaryColor}
        }
    }
    Rectangle
    {
        id: rightBorder3
        x: parent.width - border.margin - border.spacing
        y: border.margin + (border.parent.height - 2 * border.margin) * 1 / 3
        width: border.lineWidth
        height: (border.parent.height - 2 * border.margin) * 2 / 3
        gradient: Gradient{
            GradientStop{position:0.0; color:border.primaryColor}
            GradientStop{position:0.2; color:border.secondaryColor}
            GradientStop{position:1.0; color:border.primaryColor}
        }
    }
    Rectangle
    {
        id: topBorder
        x:border.radius + border.margin + border.spacing
        y:border.margin
        width: parent.width - 2 * border.margin - border.radius - border.spacing
        height: border.lineWidth
        gradient: Gradient{
            GradientStop{position:0.0; color:border.primaryColor}
            GradientStop{position:0.6; color:border.secondaryColor}
            GradientStop{position:1.0; color:border.primaryColor}
        }
    }
    Rectangle
    {
        id: bottomBorder
        x:border.margin
        y:parent.height - border.margin
        width: parent.width - 2 * border.margin - border.spacing - 2 * border.lineWidth
        height: border.lineWidth
        gradient: Gradient{
            GradientStop{position:0.0; color:border.primaryColor}
            GradientStop{position:0.6; color:border.secondaryColor}
            GradientStop{position:1.0; color:border.primaryColor}
        }
    }
    Rectangle
    {
        id: leftTop
        anchors.left: leftBorder.left
        anchors.bottom: leftBorder.top
        width: border.radius * 1.4
        height: border.radius
        color:Qt.rgba(0/255,0/255,0/255, 0/255)
        transform: Rotation
        {
            origin.x: 0;
            origin.y: border.radius;
            axis { x: 0; y: 0; z: 1 }
            angle: -45
        }
        Rectangle
        {
            id: leftTopRot
            x:0
            y:leftTop.height
            width: parent.width
            height: border.lineWidth
            gradient: Gradient{
                GradientStop{position:0.0; color:border.primaryColor}
                GradientStop{position:0.6; color:border.secondaryColor}
                GradientStop{position:1.0; color:border.primaryColor}
            }
        }
        Glow {
            anchors.fill: leftTopRot
            radius: border.spacing * 3
            samples: 37
            color: "#87CEFA"
            source: leftTopRot
        }
    }
    Rectangle
    {
        id: leftTop2
        anchors.right: topBorder.left
        anchors.top: topBorder.top
        width: border.radius * 1.4
        height: border.radius
        color:Qt.rgba(0/255,0/255,0/255, 0/255)

        Rectangle
        {
            id: leftTop2Rot
            x:0
            y:0
            width: parent.width
            height: border.lineWidth
            gradient: Gradient{
                GradientStop{position:0.0; color:border.primaryColor}
                GradientStop{position:0.6; color:border.secondaryColor}
                GradientStop{position:1.0; color:border.primaryColor}
            }
            Glow {
                anchors.fill: leftTop2Rot
                radius: border.spacing * 3
                samples: 37
                color: "#87CEFA"
                source: leftTop2Rot
            }
            transform: Rotation
            {
                origin.x: leftTop2Rot.width;
                origin.y: 0;
                axis { x: 0; y: 0; z: 1 }
                angle: -45
            }
        }
    }
    /*
    Rectangle
    {
        id: rightBottom
        anchors.top: rightBorder.bottom
        anchors.right: rightBorder.left
        width: border.radius * 1.42
        height: border.radius
        color:Qt.rgba(0/255,0/255,0/255, 0/255)
        transform: Rotation
        {
            origin.x: rightBottom.width;
            origin.y: 0;
            axis { x: 0; y: 0; z: 1 }
            angle: -45
        }
        Rectangle
        {
            id: rightBottomRot
            x:0
            y:0
            width: parent.width
            height: border.lineWidth
            gradient: Gradient{
                GradientStop{position:0.0; color:border.secondaryColor}
                GradientStop{position:1.0; color:border.primaryColor}
            }
        }
        Glow {
            anchors.fill: rightBottomRot
            radius: border.spacing * 4
            samples: 37
            color: "#87CEFA"
            source: rightBottomRot
        }
    }

    Rectangle
    {
        id: rightBottom2
        anchors.bottom: bottomBorder.top
        anchors.left: bottomBorder.right
        width: border.radius * 1.42
        height: border.radius
        color:Qt.rgba(0/255,0/255,0/255, 0/255)
        transform: Rotation
        {
            origin.x: 0;
            origin.y: rightBottom2.height;
            axis { x: 0; y: 0; z: 1 }
            angle: -45
        }
        Rectangle
        {
            id: rightBottom2Rot
            x:0
            y:rightBottom2.height
            width: parent.width
            height: border.lineWidth
            gradient: Gradient{
                GradientStop{position:0.0; color:border.secondaryColor}
                GradientStop{position:1.0; color:border.primaryColor}
            }
        }
    }*/
    Glow {
        anchors.fill: leftBorder3
        radius: border.spacing * 1
        samples: 37
        color: "#87CEFA"
        source: leftBorder3
    }
    //Glow {
    //    anchors.fill: leftBorder2
    //    radius: border.spacing * 2
    //    samples: 37
    //    color: "#87CEFA"
    //    source: leftBorder2
    //}
    Glow {
        anchors.fill: leftBorder
        radius: border.spacing * 3
        samples: 37
        color: "#87CEFA"
        source: leftBorder
    }
    Glow {
        anchors.fill: rightBorder3
        radius: border.spacing * 1
        samples: 37
        color: "#87CEFA"
        source: rightBorder3
    }
    Glow {
        anchors.fill: rightBorder2
        radius: border.spacing * 2
        samples: 37
        color: "#87CEFA"
        source: rightBorder2
    }
    Glow {
        anchors.fill: rightBorder
        radius: border.spacing * 3
        samples: 37
        color: "#87CEFA"
        source: rightBorder
    }
    Glow {
        anchors.fill: topBorder
        radius: border.spacing * 3
        samples: 37
        color: "#87CEFA"
        source: topBorder
    }
    Glow {
        anchors.fill: bottomBorder
        radius: border.spacing * 3
        samples: 37
        color: "#87CEFA"
        source: bottomBorder
    }
    //Rectangle
    //{
    //    id:leftBottomPie
    //    x:0
    //    y:border.height - 2 * border.margin
    //    width: 2 * border.margin
    //    height: 2 * border.margin
    //    color:"transparent"
    //    Image {
    //        id: pieimage
    //        width: 64
    //        height: 64
    //        anchors.fill: parent
    //        source: "circle.png"
    //    }
    //    Glow {
    //        anchors.fill: pieimage
    //        radius: 3
    //        samples: 37
    //        color: "#87CEFA"
    //        source: pieimage
    //    }
    //}

}
