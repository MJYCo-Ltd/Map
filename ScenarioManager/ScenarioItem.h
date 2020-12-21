#ifndef SCENARIOITEM_H
#define SCENARIOITEM_H
/*************************************************
* Copyright(C)
* File name:    ScenarioItem
* Author:       魏晓亮
* Version:      1.0
* Date:         2020/12/18
* Description:  方案模块数据，支持清除、保存和加载
*               方案管理模块统一管理的其他模块需要实现此接口
* History:
*************************************************/
#include "ScenarioManager_global.h"
#include <QString>
#include <QList>
#include <QDir>

class ScenarioManager;
class SCENARIOMANAGER_EXPORT ScenarioItem
{
public:
    ScenarioItem();

    void setScenarioManager(ScenarioManager* mgr);
    void setName(const QString name);
    const QString name();

    virtual void clear() = 0;
    virtual void load() = 0;
    virtual void save() = 0;

protected:
    QDir dir();

protected:
    QString                 _name;
    ScenarioManager*        _mgr;
};

#endif // SCENARIOITEM_H
