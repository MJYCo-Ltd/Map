/**
  * 日历部件的一部分
  */
import QtQuick 2.4

Item {
    id:root

    property int start: 0
    property int end: 20
    property int current: 0
    property color fontColor: Qt.rgba(142/255,234/255,252/255,1)

    signal selectChange(var value)

    onCurrentChanged: refreshData()

    onEndChanged: {
        refreshData()
    }

    Component.onCompleted: {
        refreshData()
    }

    function refreshData(){
        if(current > end){
            current = end
        }
        var mycaus=[]
        for(var i=0;i<6;i++){
            mycaus[i]= end+i-5;
        }
        for(i = 6;i<12;i++){
            mycaus[i] = start +i -6;
        }
        var flag=-1;
        for(i = 2;i<9;i++){
            if(current === mycaus[i]){
                flag = i;
            }
        }
        if(flag !== -1){
            text1.text = formataNum(mycaus[flag-2])
            text2.text = formataNum(mycaus[flag-1])
            text3.text = formataNum(mycaus[flag])
            text4.text = formataNum(mycaus[flag+1])
            text5.text = formataNum(mycaus[flag+2])
        }else{
            text1.text = formataNum(current-2)
            text2.text = formataNum(current-1)
            text3.text = formataNum(current)
            text4.text = formataNum(current+1)
            text5.text = formataNum(current+2)
        }
    }

    function formataNum(num){
        return (String(num).length >= 2) ?  String(num):('0' + num);
    }

    function updateText(y){
        if(y >= root.height *0.5){
            text1.text = text2.text
            text2.text = text3.text
            text3.text = text4.text
            text4.text = text5.text
            if(Number(text5.text) === end){
                text5.text = formataNum(start)
            }else{
               var num = Number(text5.text)+1;
               text5.text = formataNum(num)
            }
        }else{

            text5.text = text4.text
            text4.text = text3.text
            text3.text = text2.text
            text2.text = text1.text

            if(Number(text1.text) === start){
                text1.text = formataNum(end)
            }else{
               var num1 = Number(text1.text)-1;
               text1.text = formataNum(num1)
            }
        }
        current = Number(text3.text)
        root.selectChange(current)
    }

    MouseArea{
        anchors.fill: parent
        onWheel: {
            var datl = wheel.angleDelta.y/120;//一刻滚轮代表正负120度，所以除以120等于1或者-1
            if(datl>0){
                updateText(root.height *0.4)
            }else{
                updateText(root.height *0.6)
            }
        }
        onClicked: updateText(mouse.y)

    }

    Column{
        id: column
        anchors.fill: parent
        Rectangle{
            width: parent.width
            height: parent.height / 5
            color: "transparent"
            Text {
                id: text1
                color: fontColor//"#be8181"
                anchors.centerIn: parent
                font.pixelSize: parent.height * 0.2
                text: "<b>2002</b>"
            }
        }
        Rectangle{
            id: rect2
            width: parent.width
            height: parent.height / 5
            color: "transparent"
            Text {
                id: text2
                color: fontColor//"#be8181"
                //anchors.fill: parent
                anchors.centerIn: parent
                font.pixelSize: parent.height *0.3
                text: "2002"
            }
        }


        Rectangle{
            id:rect3
            width: parent.width
            height: parent.height / 5
            color: "transparent"
            Text {
                id: text3
                anchors.centerIn: parent
                font.pixelSize: parent.height* 0.5
                text: "<b>2002</b>"
                color:fontColor//"#fd7d2c"
            }
        }

        Rectangle{
            width: parent.width
            height: parent.height / 5
            color: "transparent"
            Text {
                id: text4
                color: fontColor//"#be8181"
                anchors.centerIn: parent
                font.pixelSize: parent.height * 0.3
                text: "<b>2002</b>"
            }
        }
        Rectangle{
            width: parent.width
            height: parent.height / 5
            color: "transparent"
            Text {
                id: text5
                color: fontColor//"#be8181"
                anchors.centerIn: parent
                font.pixelSize: parent.height *0.2
                text: "<b>2002</b>"
            }
        }
    }
}
