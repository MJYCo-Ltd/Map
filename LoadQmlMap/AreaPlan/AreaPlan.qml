import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle{
    id:areaPlan
    property int margin: defaultStyle.margin
    property int layerHeight: 32
    width:360
    height:700
    color: "transparent"
    Row{
        x:margin
        y:margin
        spacing: margin
        Column{
            id:colLayer
            spacing: margin
            Rectangle{
                color:"transparent"
                width:192
                height: layerHeight
                Row{
                    Button { // 新建图层按钮
                        id : buttonNew
                        width: 96
                        height:layerHeight
                        font.family: defaultStyle.fontFamilyCN
                        font.pointSize: defaultStyle.fontSizeSmall
                        //visible:false
                        text: qsTr("New")
                        onClicked: {
                            //$app.areaPlanManager.newPlan()
                        }
                    }
                    // 编辑模式选框
                    Text {
                        id: textEdit
                        width: 64
                        height:layerHeight
                        font.family: defaultStyle.fontFamilyCN
                        font.pointSize: defaultStyle.fontSizeSmall
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight
                        text: qsTr("Edit")
                    }
                    CheckBox{
                        id:checkEditMode
                        width:layerHeight
                        height:layerHeight
                        checked:false
                    }
                }
            }
            ComboBox { // 规划图选择
                id:comboxAreaPlan
                currentIndex: -1
                model: $app.areaPlanManager().planNames
                width: 192
                height:layerHeight
                delegate:ItemDelegate {
                    width: 192
                    contentItem: Text {
                        leftPadding:3
                        text: modelData
                        font.family: defaultStyle.fontFamilyCN
                        font.pointSize: defaultStyle.fontSizeSmall
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                    }
                }
                contentItem: Text {
                    leftPadding: 5
                    text: comboxAreaPlan.displayText
                    font.family: defaultStyle.fontFamilyCN
                    font.pointSize: defaultStyle.fontSizeSmall
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                onCurrentIndexChanged: {
                    $app.areaPlanManager().setCurrentPlan(comboxAreaPlan.currentIndex)
                }
            }
            ListView { // 图层（图例）列表
                id:layerView
                currentIndex: -1
                width: 192
                height:600
                highlight: Rectangle {
                    color: "lightsteelblue";
                }
                highlightFollowsCurrentItem: true;
                focus: true
                model: $app.areaPlanManager().layers
                delegate: Rectangle{
                    id:rectLayer
                    width:192
                    height: layerHeight
                    color: "transparent"
                    Row{
                        CheckBox{
                            id:checkLayerShow
                            width: layerHeight - 2
                            height: layerHeight - 2
                            checked:true
                            onCheckedChanged: {
                                $app.areaPlanManager().setLayerVisible(index, checkLayerShow.checked)
                            }
                        }
                        Rectangle{
                            color: "transparent"
                            width: 160
                            height: layerHeight
                            Rectangle{
                                color: "transparent"
                                width: 64
                                height: layerHeight
                                Image{
                                    id:layerImage
                                    anchors.centerIn: parent
                                    width: 64
                                    height: layerHeight - 4
                                    source: "file:///" + modelData.legend
                                }
                            }
                            Text{
                                id:textLayer
                                x:64
                                padding: 5
                                width: 96
                                height: layerHeight
                                font.family: defaultStyle.fontFamilyCN
                                font.pointSize: defaultStyle.fontSizeSmall
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignLeft
                                color:modelData.color
                                text: modelData.name
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    rectLayer.ListView.view.currentIndex = index;
                                    $app.areaPlanManager().setCurrentLayer(modelData.name)
                                    $app.areaPlanManager().setEditMode(checkEditMode.checked)
                                    if (checkEditMode.checked)
                                    {
                                        $app.areaPlanManager().startEdit()
                                    }
                                }
                            }
                        }
                    }
                }
                Connections {
                    target: $app.areaPlanManager()
                    function onCurrentPlanChanged() {
                        layerView.model = $app.areaPlanManager().layers
                    }
                }
                Connections {
                    target: $app.areaPlanManager()
                    function onStartEditMode() {
                        areaPlan.visible = false
                    }
                }
            }
        }
        Rectangle{
            x: colLayer.width + margin
            width:1
            height:areaPlan.height
            color:$app.areaPlanManager().currentLayerColor()
        }
        ListView { // (指定图层的)区域多边形列表
            id:areaPolygonView
            width:128
            height:areaPlan.height
            currentIndex: -1
            model:$app.areaPlanManager().areaCount
            delegate: Rectangle{
                id:rectPolygon
                height: layerHeight
                color:"transparent"
                border.width: 1
                Row{
                    width:children.width
                    Rectangle{
                        id:rectCheckShow
                        width:layerHeight
                        height:layerHeight
                        color:"transparent"
                        border.color: $app.areaPlanManager().currentLayerColor()
                        border.width: 1
                        CheckBox{
                            id:checkPolygonShow
                            anchors.centerIn: parent
                            width:layerHeight - 2
                            height:layerHeight - 2
                            checked:true
                            onCheckedChanged: {
                                $app.areaPlanManager().setAreaVisible(index, checkPolygonShow.checked)
                            }
                        }
                    }
                    Rectangle{
                        id:rectTextIndex
                        width: 64
                        height: layerHeight
                        color:"transparent"
                        border.color: $app.areaPlanManager().currentLayerColor()
                        border.width: 1
                        Text{
                            anchors.fill: parent
                            font.family: defaultStyle.fontFamilyCN
                            font.pointSize: defaultStyle.fontSizeSmall
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            color: $app.areaPlanManager().currentLayerColor() //"#000000" //
                            text: index.toString()
                        }
                        MouseArea {
                            anchors.fill: parent
                            onDoubleClicked: {
                                $app.areaPlanManager().locateArea(index)
                            }
                        }
                    }
                    Rectangle{
                        id:rectButtonDelete
                        width:layerHeight
                        height:layerHeight
                        color:"transparent"
                        border.color: $app.areaPlanManager().currentLayerColor()
                        border.width: 1
                        Button{
                            id:buttonDeletePolygon
                            anchors.centerIn: parent
                            width:layerHeight - 2
                            height:layerHeight - 2
                            font.family: defaultStyle.fontFamilyCN
                            font.pointSize: defaultStyle.fontSizeSmall
                            text:"x"
                            onClicked:{
                                $app.areaPlanManager().removeArea(index)
                            }
                        }
                    }
                }
            }
            //Connections {
            //    target: $app.areaPlanManager()
            //    function onAreaListChanged() {
            //        areaPolygonView.model = $app.areaPlanManager().areaCount()
            //        console.log("areaPolygonView onAreaListChanged")
            //    }
            //}
        }
    }
    Component.onCompleted:{
        $app.areaPlanManager().setCurrentPlan(comboxAreaPlan.currentIndex)
    }
}
