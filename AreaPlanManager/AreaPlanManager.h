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
#include "AreaPlan.h"
#include <QQmlListProperty>
#include <QObject>
#include <QList>

Q_DECLARE_METATYPE(QQmlListProperty<AreaPlan>)
//class AreaPlan;
class ISceneGraph;
class AreaPolygon;
class AREAPLANMANAGER_EXPORT AreaPlanManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<AreaPlan> planList READ planList NOTIFY planListChanged)
public:
    AreaPlanManager(QObject* parent = nullptr);
	~AreaPlanManager();

    Q_INVOKABLE void setSceneGraph(ISceneGraph*);
    Q_INVOKABLE void load(QString dirPath);
    Q_INVOKABLE void save();

    Q_INVOKABLE AreaPlan* plan(QString name);
    QQmlListProperty<AreaPlan> planList();

    AreaPlan* currentPlan();
    void setCurrentPlan(QString planName);

public slots:
    // 响应区域编辑器，在当前规划方案=>当前图层添加多边形区域
    void onAddArea(AreaPolygon*);

signals:
    void planListChanged();

private:
    void addPlan(QString planDir);
	void clear();
	bool has(QString);
	bool has(AreaPlan*);
	bool isPlanDir(QString dirPath);
private:
    QList<AreaPlan*>		_itemList;
    AreaPlan*       		_currentPlan;
};
