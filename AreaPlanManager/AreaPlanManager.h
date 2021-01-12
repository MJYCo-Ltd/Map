#pragma once
/*************************************************
* Copyright(C)
* File name:    AreaPlanManager
* Author:       魏晓亮
* Version:      1.0
* Date:         2020/06/21
* Description:  区域规划管理
* History:
*************************************************/
#include "AreaPlanManager_global.h"
#include "AreaPlanLayer.h"
#include "AreaPlan.h"
#include "../ScenarioManager/ScenarioItem.h"
#include <QQmlListProperty>
#include <QObject>
#include <QList>

Q_DECLARE_METATYPE(QQmlListProperty<AreaPlan>)
Q_DECLARE_METATYPE(QQmlListProperty<AreaPlanLayer>)
//Q_DECLARE_METATYPE(QQmlListProperty<AreaPolygon>)

//class AreaPlan;
class ISceneGraph;
class AreaPolygon;
class AREAPLANMANAGER_EXPORT AreaPlanManager : public QObject, public ScenarioItem
{
    Q_OBJECT
    Q_PROPERTY(QStringList planNames READ planNames NOTIFY planListChanged)
    Q_PROPERTY(QQmlListProperty<AreaPlan> plans READ plans NOTIFY planListChanged)
    Q_PROPERTY(QQmlListProperty<AreaPlanLayer> layers READ layers NOTIFY layerListChanged)
    //Q_PROPERTY(QQmlListProperty<AreaPolygon> areas READ areas NOTIFY areaListChanged)
public:
    AreaPlanManager(QObject* parent = nullptr);
	~AreaPlanManager();

    Q_INVOKABLE void setSceneGraph(ISceneGraph*);

    // create   : 新建方案时调用
    virtual void create(){}
    // clear    : 切换方案前清除，以便加载新方案
    virtual void clear();
    // load     : 加载方案时调用
    virtual void load();
    // save     : 保存方案时调用
    Q_INVOKABLE void save();

    Q_INVOKABLE AreaPlan* plan(QString name);
    Q_INVOKABLE AreaPlan* plan(int index);
    QList<AreaPlan*> planList();
    QStringList planNames();
    QQmlListProperty<AreaPlan> plans();
    QQmlListProperty<AreaPlanLayer> layers();
    //QQmlListProperty<AreaPolygon> areas();
    Q_INVOKABLE int areaCount();
    Q_INVOKABLE void locateArea(int index);
    Q_INVOKABLE void setAreaVisible(int index, bool);

    Q_INVOKABLE bool isCurrentPlan(QString planName);
    AreaPlan* currentPlan();
    Q_INVOKABLE void setCurrentPlan(AreaPlan*);
    Q_INVOKABLE void setCurrentPlan(int index);
    Q_INVOKABLE void setCurrentPlan(QString planName);
    AreaPlanLayer* currentLayer();
    Q_INVOKABLE void setCurrentLayer(AreaPlanLayer*);
    Q_INVOKABLE void setCurrentLayer(int index);
    Q_INVOKABLE void setCurrentLayer(QString layerName);
    Q_INVOKABLE QColor currentLayerColor();
    Q_INVOKABLE void setEditMode(bool editMode);
    Q_INVOKABLE void startEdit();

public slots:
    // 响应区域编辑器，在当前规划方案=>当前图层添加多边形区域
    void onAddArea(AreaPolygon*);

signals:
    void planListChanged();
    void layerListChanged();
    void areaListChanged();
    void currentPlanChanged();
    void currentLayerChanged();
    void startEditMode();

private:
    void addPlan(QString planDir);
    //void clear();
	bool has(QString);
	bool has(AreaPlan*);
	bool isPlanDir(QString dirPath);

    AreaPolygon* area(int index);
    void locateArea(AreaPolygon*);
private:
    QList<AreaPlan*>		_itemList;
    AreaPlan*       		_currentPlan;
    // 使用临时的列表是为了统一外部接口，避免外部调用manager以为的其他（空）对象时造成崩溃
    QList<AreaPlanLayer*>	_layerList;     // temp
    QList<AreaPolygon*>     _polygonList;   // temp
};
