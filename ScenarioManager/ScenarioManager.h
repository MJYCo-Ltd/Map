#pragma once
/*************************************************
* Copyright(C)
* File name:    ScenarioManager
* Author:       魏晓亮
* Version:      1.0
* Date:         2020/12/18
* Description:  负责方案文件目录的管理、方案的加载和保存
*               方案包含
*               ①方案基础信息(Scenario：方案名称、目录、以及描述、图片等)和
*               ②方案模块数据(ScenarioItem接口实现：演示动画、区域规划等)
* History:
*************************************************/
#include "ScenarioManager_global.h"
#include "Scenario.h"
#include <QQmlListProperty>
#include <QObject>
#include <QString>
#include <QList>
#include <QDir>

Q_DECLARE_METATYPE(QQmlListProperty<Scenario>)
class ScenarioItem;
class ISceneGraph;
class SCENARIOMANAGER_EXPORT ScenarioManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Scenario> scenarios READ scenarios NOTIFY scenarioListChanged)    
    Q_PROPERTY(QQmlListProperty<Scenario> favorites READ favorites NOTIFY favoriteListChanged)
public:
    ScenarioManager(QObject* parent=nullptr);
    //ScenarioManager(const ScenarioManager&);
    //void operator=(const ScenarioManager&);
    Q_INVOKABLE void setSceneGraph(ISceneGraph*);   // 设置场景图形接口，以实现方案场景定位功能
    void setDir(QString dirPath);                   // 设置方案目录，并初始化
    QDir dir() const;

    Q_INVOKABLE Scenario* currentScenario() const;
    Q_INVOKABLE void setCurrentScenario(QString name);
    QList<Scenario*> scenarioList() const;
    QList<Scenario*> favoriteList() const;
    QQmlListProperty<Scenario> scenarios();
    QQmlListProperty<Scenario> favorites();
    bool contains(QString name);
    Q_INVOKABLE Scenario* scenario(QString name);

    // 返回值
    //        0 : 空指针
    //       -1 : 不存在
    //       -2 : 已存在
    //       -3 : 文件目录操作失败
    Q_INVOKABLE int addScenario(QString name);
    Q_INVOKABLE int removeScenario(QString name);
    int removeAllScenario();
    Q_INVOKABLE int addFavorite(QString name);
    Q_INVOKABLE int removeFavorite(QString name);

    Q_INVOKABLE void load();
    Q_INVOKABLE void save();
    void loadFavorites();
    void saveFavorites();
    void saveAs(QString newName);

    QList<ScenarioItem*> itemList() const;
    Q_INVOKABLE void addItem(ScenarioItem*);
    bool contains(ScenarioItem*);
    //void removeScenarioItem(ScenarioItem*);

    //ISceneGraph* sceneGraph() const;
public slots:
    void locate();
signals:
    void scenarioListChanged(QQmlListProperty<Scenario>);
    void favoriteListChanged(QQmlListProperty<Scenario>);

protected:
    void init();
    void clear();
    void clearScenarioList();
    void clearFavoriteList();
protected:
    QDir                    _dir;
    Scenario*               _currentScenario;
    QList<Scenario*>        _scenarioList;
    QList<Scenario*>        _favoriteList;
    QList<ScenarioItem*>    _itemList;
    ISceneGraph*            _sceneGraph;
};

