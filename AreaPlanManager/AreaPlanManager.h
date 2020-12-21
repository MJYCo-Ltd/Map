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
class AREAPLANMANAGER_EXPORT AreaPlanManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<AreaPlan> itemList READ itemList NOTIFY itemListChanged)
public:
    AreaPlanManager(QObject* parent = nullptr);
	~AreaPlanManager();

	void load(QString dirPath);
	void save();
    Q_INVOKABLE AreaPlan* item(QString name);
    QQmlListProperty<AreaPlan> itemList();
signals:
    void itemListChanged();
private:
	void addItem(QString planDir);
	void clear();
	bool has(QString);
	bool has(AreaPlan*);
	bool isPlanDir(QString dirPath);
private:
    QList<AreaPlan*>		_itemList;
};
