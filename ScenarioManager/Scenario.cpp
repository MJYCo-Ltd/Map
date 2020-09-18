#include "Scenario.h"
#include "ScenarioManager.h"

Scenario::Scenario(ScenarioManager* mgr, QString name):
    _mgr(mgr), _name(name)
{
    //QDir dir = _mgr->dir();
    //QDir scenarioDir(dir.path() + "/" + name);
    //QStringList fileList = scenarioDir.entryList(QDir::Files);
    //bool isScenarioDir = false;
    //foreach (QString file, fileList)
    //{
    //    if (file.endsWith(".snr"))
    //        isScenarioDir = true;
    //}
    //if (!isScenarioDir)
    //    delete this;
}

QString Scenario::name()
{
    return _name;
}

QDir Scenario::dir()
{
    return QDir(_mgr->dir().path() + "/" + name());
}

QString Scenario::imageFilePath()
{
    return dir().path() + "/" + name() + ".png";
}

