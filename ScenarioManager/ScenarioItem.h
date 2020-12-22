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

//class ScenarioManager;
class SCENARIOMANAGER_EXPORT ScenarioItem
{
public:
    ScenarioItem();

    // 不同模块名称应当不同
    void setName(const QString name);
    const QString name();
    // 设置模块使用的文件根目录（目录名同方案名称）
    bool setScenarioDir(QString);

    // create   : 新建方案时调用
    virtual void create() = 0;
    // clear    : 切换方案前清除，以便加载新方案
    virtual void clear() = 0;
    // load     : 加载方案时调用
    virtual void load() = 0;
    // load     : 保存方案时调用
    virtual void save() = 0;

protected:
    QDir dir();
signals:
    void dataChanged();

protected:
    QString                 _name;
    QDir                    _scenarioDir;
    QDir                    _dir;
    //ScenarioManager*        _mgr;
};

#endif // SCENARIOITEM_H
