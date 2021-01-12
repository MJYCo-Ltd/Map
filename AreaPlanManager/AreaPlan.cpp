#include "AreaPlan.h"
#include "AreaPlanLayer.h"
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QDebug>

AreaPlan::AreaPlan(QObject* parent)
{
	_currentLayer = nullptr; 
}

AreaPlan::~AreaPlan()
{}

void AreaPlan::load(QString jsonDir)
{
	setDirPath(jsonDir);
	load();
    //setVisible(false);
	return;
}

QString AreaPlan::dirPath()
{
    return _dirPath;
}

void AreaPlan::setDirPath(QString dirPath)
{
	_dirPath = dirPath;
}

QString AreaPlan::layerDirPath()
{
	return _dirPath + "/Layers";
}

void AreaPlan::load()
{
    QDir dir(dirPath());
	if (!dir.exists())
		return;
    QString _jsonFileName = jsonFileName(dirPath());
	load(_jsonFileName, "*.json");
	return;
}

void AreaPlan::load(QString fn, QString format)
{
	_jsonFileName = fn;
	if (format.endsWith(".json") || format.endsWith(".JSON"))
	{
        QFile file(QString(dirPath() + "/" + _jsonFileName));
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			qDebug() << "File open error";
		}
		QJsonParseError jsonError;
		QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &jsonError);
		if (!document.isNull() && (jsonError.error == QJsonParseError::NoError))
		{
			if (document.isArray())
			{
				QJsonArray array = document.array();
				for (int i = 0; i < array.size(); ++i)
				{
					QJsonValue value = array.at(i);
					if (value.isObject())
					{
						QJsonObject object_layer = value.toObject();
						QString name = object_layer.value("name").toString();
						QString legend = object_layer.value("legend").toString();
						QColor color(object_layer.value("color").toString());
                        //this->addLayer(name, icon(legend), color);
                        //qDebug() << "legend:" << legend;
                        this->addLayer(name, legend, color);
					}
				}
			}
		}
	}
}

void AreaPlan::save()
{
    QDir dir(dirPath());
	if (!dir.exists())
		return;
    // QString _jsonFileName = jsonFileName(dirPath());
	// save(_jsonFileName, "*.json");
    foreach (AreaPlanLayer* one, _layerList)
	{
		one->save(layerDirPath() + "/" + one->name() + ".json");
	}
	return;	
}

void AreaPlan::save(QString fn, QString format = "*.json")
{
	// 此文件不会被程序修改，暂时不需要保存
}

QList<AreaPlanLayer*> AreaPlan::layerList()
{
    return _layerList;
}
/*
QQmlListProperty<AreaPlanLayer> AreaPlan::layers()
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    return QQmlListProperty<AreaPlanLayer>(this, _layerList);
#else
    return QQmlListProperty<AreaPlanLayer>(this, &_layerList);
#endif
    //return _layerList;
}
*/
AreaPlanLayer* AreaPlan::currentLayer()
{
	return _currentLayer;
}

void AreaPlan::setCurrentLayer(QString layerName)
{
	AreaPlanLayer* res = layer(layerName);
	if (res)
		_currentLayer = res;
}

void AreaPlan::setVisible(bool visible)
{
    for (int i = 0; i < layerList().count(); i++)
    {
        layerList()[i]->setVisible(visible);
    }
}

QString AreaPlan::name()
{
	return _name;
}

void AreaPlan::setName(QString name)
{
    _name = name;
}

QString AreaPlan::jsonFileName(QString dirPath)
{
	QDir dir(dirPath);
	QStringList entryList = dir.entryList(QStringList("*.json"), QDir::Files);
	if (entryList.count() > 0)
		return entryList[0];
	else
		return "";
}

AreaPlanLayer* AreaPlan::layer(QString layerName)
{
    foreach(AreaPlanLayer* one, _layerList)
	{
		if (QString::compare(one->name(), layerName) == 0)
		{
			return one;
		}
	}
	return nullptr;
}

AreaPlanLayer* AreaPlan::layer(int index)
{
    if (layerList().count() > index)
        return layerList()[index];
    else
        return nullptr;
}

AreaPlanLayer* AreaPlan::addLayer(QString name, QString legend, QColor color)
{
    AreaPlanLayer* res = layer(name);
    if (nullptr == layer(name))
    {
        res = new AreaPlanLayer();
        res->setName(name);
        res->setLegend(dirPath() + "/" + legend);
        res->setColor(color);
        res->load(layerDirPath() + "/" + name + ".json");
        _layerList.append(res);
        emit layerListChanged();
    }
    return res;
}
/*
AreaPlanLayer* AreaPlan::addLayer(QString name, QIcon legend, QColor color)
{
	AreaPlanLayer* res = layer(name);
	if (nullptr == layer(name))
	{
		res = new AreaPlanLayer();
		res->setName(name);
		res->setLegend(legend);
		res->setColor(color);
		res->load(layerDirPath() + "/" + name + ".json");
		_layerList.append(res);
        emit layerListChanged();
	}
	return res;
}

QIcon AreaPlan::icon(QString filename)
{
    QString iconFilePath = dirPath() + "/" + filename;
	return QIcon(iconFilePath);
}*/
