#pragma once
/*************************************************
* Copyright(C)
* File name:    AreaPlan
* Author:       魏晓亮
* Version:      1.0
* Date:         2020/06/23
* Description:  
* History:
*************************************************/
#include "AreaPlanManager_global.h"
#include "AreaPlanLayer.h"
#include <QQmlListProperty>
#include <QObject>
#include <QList>
#include <QIcon>
#include <QColor>

//Q_DECLARE_METATYPE(QQmlListProperty<AreaPlanLayer>)
//class AreaPlanLayer;
class AREAPLANMANAGER_EXPORT AreaPlan : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    //Q_PROPERTY(QQmlListProperty<AreaPlanLayer> layers READ layers NOTIFY layerListChanged)
public:
    AreaPlan(QObject* parent = nullptr);
	virtual ~AreaPlan();

	void load(QString dirPath);
	void save();

	QString name();
    void setName(QString name);
    QList<AreaPlanLayer*> layerList();
    //QQmlListProperty<AreaPlanLayer> layers();
	AreaPlanLayer* currentLayer(); 
	void setCurrentLayer(QString layerName);
    //void setVisible(bool);
    //void setLayerVisible(QString layerName, bool);
signals:
    void nameChanged();
    void layerListChanged();
private:
	AreaPlanLayer* layer(QString layerName);
    AreaPlanLayer* addLayer(QString name, QString legend, QColor color = QColor(255,255,255));
    //AreaPlanLayer* addLayer(QString name, QIcon legend, QColor color = QColor(255,255,255));
	QIcon icon(QString filename);
	QString jsonFileName(QString dirPath);

    QString dirPath();
	void setDirPath(QString dirPath);
	QString layerDirPath();
	void load();
	void load(QString fn, QString format); // e.g. format = "*.json"
	void save(QString fn, QString format);
private:
	QList<AreaPlanLayer*>		_layerList;
	AreaPlanLayer*				_currentLayer;
	QString						_dirPath;
	QString						_jsonFileName;
	QString						_name;
};
