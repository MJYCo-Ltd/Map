#include "AreaPlanLayer.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QFile>

AreaPlanLayer::AreaPlanLayer()
{
}

AreaPlanLayer::~AreaPlanLayer()
{
}

QString AreaPlanLayer::name()
{
	return _name;
}

void AreaPlanLayer::setName(QString name)
{
	_name = name;
}

QIcon AreaPlanLayer::legend()
{
	return _legend;
}

void AreaPlanLayer::setLegend(QIcon legend)
{
	_legend = legend;
}

QColor AreaPlanLayer::color()
{
	return _color;
}

void AreaPlanLayer::setColor(QColor color)
{
	_color = color;
}

void AreaPlanLayer::load(QString jsonFilePath)
{
	QFile file(jsonFilePath);
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
			int debug_point_not_enough = 0;
			QJsonArray arrayArea = document.array();
			for (int i = 0; i < arrayArea.size(); i++)
			{
				QJsonValue value = arrayArea.at(i);
				if (value.isArray())
				{
					QJsonArray arrayPoint = value.toArray();
					if (arrayPoint.size() < 3)
					{
						debug_point_not_enough++;
						qDebug() << "file:" << jsonFilePath << ", " << debug_point_not_enough << ":point not enough!";
						continue;
					}

					for (int j = 0; j < arrayPoint.size(); j++) 
					{
						QJsonValue value = arrayPoint.at(j);
						if (value.isObject())
						{
							double x = value.toObject().value("lon").toDouble();
							double y = value.toObject().value("lat").toDouble();
							double z = value.toObject().value("alt").toDouble();
                            //addPoint(osg::Vec3d(x,y,z));
						}
                    }
				}
			}
		}
	}
}

void AreaPlanLayer::save(QString jsonFilePath)
{
	QFile file(jsonFilePath);
	if (!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))
	{
		qDebug() << "file error";
		return;
	}
	QTextStream in(&file);

    QJsonArray areaArray;

	QJsonDocument document;
	document.setArray(areaArray);

	in << document.toJson();
	file.close();
}

