#pragma once
/*************************************************
* Copyright(C)
* File name:    Scenario
* Author:       魏晓亮
* Version:      1.0
* Date:         2020/12/18
* Description:  方案基础信息
*               名称、目录需要构造时设置好以便查询管理
* History:
*************************************************/

#include "ScenarioManager_global.h"
#include <QVector3D>
#include <QObject>
#include <QDir>

class ScenarioManager;
class SCENARIOMANAGER_EXPORT Scenario : public QObject
{
    //friend ScenarioManager;
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString image READ image WRITE setImage NOTIFY imageChanged)
//private:
public:
    Scenario(QObject *parent = nullptr);
    void setScenarioManager(ScenarioManager*);
    QDir dir();
    void setDir(QDir);
    Q_INVOKABLE QString name();
    Q_INVOKABLE void setName(QString);
    Q_INVOKABLE QString image();
    Q_INVOKABLE void setImage(QString);
    // 方案中心位置：经纬高
    void setLocation(QVector3D);
    QVector3D location();

    void save();
    void load();
signals:
    void nameChanged();
    void imageChanged();
public slots:
private:
    QDir                _dir;
    QString             _name;
    QString             _image;
    QVector3D           _location;
    ScenarioManager*    _mgr;
};
