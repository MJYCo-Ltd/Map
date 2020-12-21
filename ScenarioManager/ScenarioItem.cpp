#include "ScenarioItem.h"
#include "ScenarioManager.h"

ScenarioItem::ScenarioItem()
{

}

void ScenarioItem::setScenarioManager(ScenarioManager* mgr)
{
    _mgr = mgr;
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
    QString dirPath = _mgr->currentScenario()->dir().path() + "/" + name();
    QDir dir(dirPath);
    if ( ! dir.exists())
    {
        _mgr->currentScenario()->dir().mkdir(name());
    }
    return dir;
}

