#include "ScenarioItem.h"
#include "ScenarioManager.h"

ScenarioItem::ScenarioItem()
{

}

bool ScenarioItem::setScenarioDir(QString dirPath)
{
    _scenarioDir = QDir(dirPath);
    return _scenarioDir.exists();
}

void ScenarioItem::setName(const QString name)
{
    _name = name;
}

const QString ScenarioItem::name()
{
    return _name;
}

QDir ScenarioItem::dir()
{
    QString dirPath = _scenarioDir.path() + "/" + name();
    QDir _dir(dirPath);
    if ( ! _dir.exists())
    {
        _scenarioDir.mkdir(name());
    }
    return _dir;
}

