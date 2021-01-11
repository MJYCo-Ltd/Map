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
//class AreaPlan;
class ISceneGraph;
class AreaPolygon;
class AREAPLANMANAGER_EXPORT AreaPlanManager : public QObject, public ScenarioItem
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<AreaPlan> plans READ plans NOTIFY planListChanged)
    Q_PROPERTY(QQmlListProperty<AreaPlanLayer> layers READ layers NOTIFY layerListChanged)
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
    QList<AreaPlan*> planList();
    QQmlListProperty<AreaPlan> plans();
    QQmlListProperty<AreaPlanLayer> layers();

    Q_INVOKABLE bool isCurrentPlan(QString planName);
    Q_INVOKABLE AreaPlan* currentPlan();
    Q_INVOKABLE void setCurrentPlan(QString planName);
    Q_INVOKABLE void setCurrentLayer(QString layerName);
    Q_INVOKABLE void setEditMode(bool editMode);
    Q_INVOKABLE void startEdit();

public slots:
    // 响应区域编辑器，在当前规划方案=>当前图层添加多边形区域
    void onAddArea(AreaPolygon*);

signals:
    void planListChanged();
    void layerListChanged();
    void currentPlanChanged();

private:
    void addPlan(QString planDir);
    //void clear();
	bool has(QString);
	bool has(AreaPlan*);
	bool isPlanDir(QString dirPath);
private:
    QList<AreaPlan*>		_itemList;
    QList<AreaPlanLayer*>	_layerList;
    AreaPlan*       		_currentPlan;
};
