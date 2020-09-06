import QtQuick 2.15
import QtGraphicalEffects 1.0

Rectangle {
    property int margin: 10
    property int radius: margin
    property int titleWidth: defaultStyle.titleWidth
    property int titleHeight: defaultStyle.titleHeight
    property color backgroundColor : defaultStyle.backgroundColor
    color: backgroundColor
    Canvas {
        x : margin
        y : margin
        width: parent.width - 2 * margin
        height: parent.height - 2 * margin
        contextType: "2d"

        onPaint: {
            context.lineWidth = 1
            context.strokeStyle = defaultStyle.secondaryColor
            context.fillStyle = defaultStyle.primaryColor
            context.beginPath();
            context.moveTo(0, radius)
            context.lineTo(0, height - radius)
            context.lineTo(radius, height)
            context.lineTo(width - radius, height);
            context.lineTo(width,  height - radius);
            context.lineTo(width, radius);
            context.lineTo(width - radius, 0);
            context.lineTo(radius, 0);
            context.closePath();
            context.fill()
            context.stroke();

            var offset = 4
            context.lineWidth = 2
            context.moveTo(offset, height - radius - offset)
            context.lineTo(radius + offset, height - offset)

            context.moveTo(width - radius - offset, height - offset);
            context.lineTo(width - offset,  height - radius - offset);

            context.moveTo(width - offset, radius + offset);
            context.lineTo(width - radius - offset, offset);

            context.moveTo(radius + offset, offset);
            context.lineTo(offset, radius + offset)
            context.stroke();
        }
    }

    Rectangle {
        id:innerRect
        property int margin: parent.margin
        property int radius: margin
        color: parent.backgroundColor
        x : margin
        y : margin
        width: parent.width - 2 * margin
        height: parent.height - 2 * margin
        Canvas {
            x : margin
            y : margin
            width: parent.width - 2 * margin
            height: parent.height - 2 * margin
            contextType: "2d"
            onPaint: {
                context.lineWidth = 2
                context.strokeStyle = defaultStyle.secondaryColor
                context.fillStyle = defaultStyle.primaryColor
                context.beginPath();
                context.moveTo(0, titleHeight + radius)
                context.lineTo(0, height - radius)
                context.lineTo(radius, height)
                context.lineTo(width - radius, height);
                context.lineTo(width,  height - radius);
                context.lineTo(width, radius);
                context.lineTo(width - radius, 0);
                context.lineTo(titleWidth + 2* radius,  0);
                context.lineTo(titleWidth - 2* radius,  titleHeight);
                context.lineTo(radius,  defaultStyle.titleHeight);
                context.closePath();
                context.fill()

                context.lineWidth = 4
                context.moveTo(radius + titleWidth / 6,  titleHeight - context.lineWidth);
                context.lineTo(radius + titleWidth * 5 / 6,  titleHeight - context.lineWidth);
                context.stroke();
            }
        }
    }

}
