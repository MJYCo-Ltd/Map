#ifndef AREAPOLYGONEDITOR_H
#define AREAPOLYGONEDITOR_H
/*************************************************
* Copyright(C)
* File name:    AreaPolygonEditor
* Author:       魏晓亮
* Version:      1.0
* Date:         2021/01/05
* Description:  AreaPolygon的三维编辑器
*               编辑过程中更新缓存数据，实时显示已添加的点和线
*               编辑完成清空缓存，发送完整的多边形数据
* History:
*************************************************/
#include "AreaPlanManager_global.h"
#include <SceneGraph/IWindow.h>
#include <Plot/Map/IMapPoint.h>
#include <Plot/Map/IMapLine.h>
#include <Plot/Map/IMapPolygon.h>
#include <Plot/Map/IMapLayer.h>
#include <QObject>
#include "AreaPlanLayer.h"
#include "AreaPolygon.h"

class AREAPLANMANAGER_EXPORT AreaPolygonEditor : public QObject, public IWindowMessageObserver
{
    Q_OBJECT
private:
    AreaPolygonEditor();
    static AreaPolygonEditor* _self;
public:
    static AreaPolygonEditor* getInstance()
    {
        if(_self == nullptr)
            _self = new AreaPolygonEditor();
        return _self;
    }

    // 设置场景接口(必须)
    void setSceneGraph(ISceneGraph*);
    //ISceneGraph* sceneGraph();

    // 开始编辑：接收鼠标事件，直到点击鼠标右键完成编辑
    void start();

    // 处理鼠标事件
    void MouseDown(MouseButtonMask, int, int);  // 左键添加点，右键结束编辑
    void MouseMove(MouseButtonMask, int, int);  // 移动时要绘制当前鼠标位置和上一个点的连线

    void createPolygon(AreaPolygon*, AreaPlanLayer*);
    void deletePolygon(AreaPolygon*, AreaPlanLayer*);
signals:
    void addArea(AreaPolygon*);                 // 编辑完成后发出此信号

public slots:

private:
    void stop();
    void setEnable(bool);
    bool isEnable();
    void clear();

private:
    ISceneGraph*        _sceneGraph;
    bool                _enable;
    // 编辑缓存
    QList<IMapPoint*>   _points;
    QList<IMapLine*>    _lines;
    IMapPolygon*        _polygon;
    IMapLayer*          _layer;
};

#endif // AREAPOLYGONEDITOR_H
