#include "Scenario.h"
#include "ScenarioManager.h"

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

void Scenario::save()
{

}

void Scenario::load()
{

}
