#include "ScenarioItem.h"
#include "ScenarioManager.h"

ScenarioItem::ScenarioItem(QString name, ScenarioManager* mgr)
    :_name(name), _mgr(mgr)
{

}

const QString ScenarioItem::name()
{
    return _name;
}

void ScenarioItem::saveAs(const QString newName)
{
    _name = newName;
    save();
}

QDir ScenarioItem::dir()
{
    QString dirPath = _mgr->scenarioDir().path() + "/" + name();
    QDir dir(dirPath);
    if ( ! dir.exists())
    {
        _mgr->scenarioDir().mkdir(name());
    }
    return dir;
}

