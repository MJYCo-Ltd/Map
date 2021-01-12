#include "Scenario.h"
#include "ScenarioManager.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QDebug>

Scenario::Scenario(QObject *parent) : QObject(parent)
{
}

void Scenario::setScenarioManager(ScenarioManager* mgr)
{
    _mgr = mgr;
}

QDir Scenario::dir()
{
    if(_mgr == nullptr)
        return QDir();

    _dir = QDir(_mgr->dir().path() + "/" + name());
    return _dir;
}

void Scenario::setDir(QDir dir)
{
    _dir = dir;
}

QString Scenario::name()
{
    return _name;
}

void Scenario::setName(QString name)
{
    _name = name;
}

QString Scenario::image()
{
    return _image;
}

void Scenario::setImage(QString image)
{
    _image = image;
}

void Scenario::setLocation(QVector3D loc)
{
    _location = loc;
}

QVector3D Scenario::location()
{
    return _location;
}

void Scenario::save()
{

}

void Scenario::load()
{
    QFile file(dir().path() + "/location.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "File open error";
    }

    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &jsonError);
    if (!document.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        if (document.isObject())
        {
            float lon = document.object().value("lon").toDouble();
            float lat = document.object().value("lat").toDouble();
            float alt = document.object().value("alt").toDouble();
            setLocation(QVector3D(lon, lat, alt));
        }
    }
}
