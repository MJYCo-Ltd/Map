/**
  * 日期时间选择控件
  */
import QtQuick 2.9
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.0
import QtQml 2.0

Rectangle
{
    id:root
    width: 490
    height: 420
    color: "transparent"
    property int margin: defaultStyle.margin
    BorderImg3{
        id: borderDataTime
        anchors.fill: parent
    }
    property int year:2020
    property int month: 1
    property int day: 1
    property int hour: 1
    property int minute: 1
    property int second: 1

    property var local: Qt.locale()

    signal dataTime(var value)

    function setDataTime(value){
        //console.log(value)
        var s = String(value).split('/')
        year = (s[0]>3000 ? 3000:s[0])
        month=(s[1]>12 ? 12:s[1])
        day= (s[2]>31 ? 31:s[2])
        hour = (s[3]>23 ? 23:s[3])
        minute=(s[4]>59 ? 59:s[4])
        second = (s[5]>59 ? 59:s[5])
    }

    function isLeapYear(year) {
        var cond1 = year % 4 == 0;
        var cond2 = year % 100 != 0;
        var cond3 = year % 400 ==0;
        var cond = cond1 && cond2 || cond3;
        if(cond) {
            return true;
        } else {
            return false;
        }
    }

    function timeChange(){
        var normalYear=[31,28,31,30,31,30,31,31,30,31,30,31];
        var leapYear=[31,29,31,30,31,30,31,31,30,31,30,31];
        if(isLeapYear(yearButton.current)){
            dayButton.end = leapYear[monthButton.current-1]
        }else{
            dayButton.end = normalYear[monthButton.current-1]
        }
    }
    //阴影
    layer.enabled: true
    layer.effect: DropShadow{
        verticalOffset:2
        color: "#80000000"
        radius: 1
        samples: 3
    }
    Column{
        id: column
        x: margin * 2
        y: margin * 2
        width: parent.width - margin * 4
        height: parent.height - margin * 4
        spacing: 9
        //请选择时间
        Text{
            width: parent.width
            height: 30
            id:tipsText
            text: qsTr("请选择时间")
            anchors.horizontalCenterOffset: 0
            color: defaultStyle.fontColor
            font.pointSize: 12
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignBottom
        }
        //line
        Rectangle{
            id:line1
            anchors.horizontalCenter: parent.horizontalCenter            
            width: parent.width - margin
            height: 1
            color: defaultStyle.borderColor
        }
        Row{
            width: parent.width
            height: 20
            Rectangle{
                color: "transparent"
                height: 20
                width: (parent.width - margin) /6.0
                Text {
                    font.pointSize: 10
                    anchors.centerIn: parent
                    color: defaultStyle.fontColor
                    text: qsTr("年")
                }
            }
            Rectangle{
                color: "transparent"
                height: 20;
                width: (parent.width - margin) /6.0;
                Text {
                    font.pointSize: 10
                    anchors.centerIn: parent
                    text: qsTr("月")
                    color: defaultStyle.fontColor
                }
            }
            Rectangle{
                color: "transparent"
                height: 20;
                width: (parent.width - margin) /6.0;
                Text {
                    font.pointSize: 10
                    anchors.centerIn: parent
                    text: qsTr("日")
                    color: defaultStyle.fontColor
                }
            }
            Rectangle{
                color: "transparent"
                height: 20;
                width: (parent.width - margin) /6.0;
                Text {
                    font.pointSize: 10
                    anchors.centerIn: parent
                    text: qsTr("时")
                    color :defaultStyle.fontColor
                }
            }
            Rectangle{
                color: "transparent"
                height: 20;
                width: (parent.width - margin) /6.0;
                Text {
                    font.pointSize: 10
                    anchors.centerIn: parent
                    text: qsTr("分")
                    color: defaultStyle.fontColor
                }

            }
            Rectangle{
                color: "transparent"
                height: 20;
                width: (parent.width - margin) /6.0;
                Text {
                    font.pointSize: 10
                    anchors.centerIn: parent
                    text: qsTr("秒")
                    color:defaultStyle.fontColor
                }
            }
        }
        Rectangle{
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - margin
            height: 1
            color: defaultStyle.borderColor
        }
        Row{
            width:  parent.width
            height:  240
            SliderDataBar{
                id: yearButton
                width:(parent.width - margin) /6.0
                height:240
                start:2000
                end :3000
                current: root.year
                onCurrentChanged: {
                    timeChange()
                }
            }
            SliderDataBar{
                id:monthButton
                width:(parent.width - margin) /6.0
                height:240
                start:1
                end :12
                current:root.month
                onCurrentChanged: {
                    timeChange()
                }
            }
            SliderDataBar{
                id:dayButton
                width:(parent.width - margin) /6.0
                height:240
                start:1
                end :31
                current:root.day
            }
            SliderDataBar{
                id:hourButton
                width:(parent.width - margin) /6.0
                height:240
                start:0
                end :23
                current:root.hour
            }
            SliderDataBar{
                id:minuteButton
                width:(parent.width - margin) /6.0
                height:240
                start:0
                end :59
                current:root.minute
            }
            SliderDataBar{
                id:secondButton
                width:(parent.width - margin) /6.0
                height:240
                start:0
                end :59
                current:root.second
            }

        }
        Rectangle{
            id:line2
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - margin
            height: 1
            color: defaultStyle.borderColor
        }
        Row{
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - margin
            layoutDirection: Qt.RightToLeft
            ButtonA{
                width: 100
                height: 28
                border.width: 0
                text:qsTr("确认")
                onClicked:{
                    root.dataTime(String(yearButton.current+'/'+
                                         monthButton.current+'/'+
                                         dayButton.current+'/'+
                                         hourButton.current+'/'+
                                         minuteButton.current+'/'+
                                         secondButton.current))
                    root.visible = false
                }
            }
            ButtonA{
                width: 100
                height: 28
                border.width: 0
                text:qsTr("现在")
                onClicked:{
                    var time = Qt.formatDateTime(new Date(), "yyyy/MM/dd/HH/mm/ss")
                    root.setDataTime(time)
                }
                color: "transparent"
                radius: 0
                //normalColor: "transparent"

            }
            ButtonA{
                width: 100
                height: 28
                border.width: 0
                text:qsTr("今天")
                onClicked:{
                    var time = Qt.formatDateTime(new Date(), "yyyy/MM/dd/00/00/00")
                    root.setDataTime(time)
                }
            }
        }
    }
}
