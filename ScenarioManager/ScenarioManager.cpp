#include "ScenarioManager.h"
#include "ScenarioItem.h"
#include <QDir>

ScenarioManager::ScenarioManager(QDir dir):_dir(dir)
{
    addTempScenario();
}

QDir ScenarioManager::dir()
{
    return _dir;
}

QDir ScenarioManager::scenarioDir()
{
    return _scenarioDir;
}


QString ScenarioManager::currentScenario()
{
    return _currentScenario;
}

void ScenarioManager::setCurrentScenario(QString name)
{
    QStringList list = scenarios();
    if (list.contains(name))
    {
        _currentScenario = name;
        _scenarioDir = dir().path() + "/" + currentScenario();
    }
}

QStringList ScenarioManager::scenarios()
{
    QDir dir = this->dir();
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    return dir.entryList();
}

bool ScenarioManager::contains(QString name)
{
    QStringList list = scenarios();
    return list.contains(name);
}

void ScenarioManager::addTempScenario()
{
    int n = 1;
    QString name = "untitled_";
    while(!addScenario(name + QString::number(n)))
    {
        n++;
    }
}

bool ScenarioManager::addScenario(QString name)
{
    if (contains(name))
        return false;
    dir().mkdir(name);
    emit scenarioListChanged();
    return true;
}

void ScenarioManager::removeScenario(QString name)
{
    dir().rmdir(name);
    emit scenarioListChanged();
}

void ScenarioManager::load()
{
    foreach (ScenarioItem* one, _itemList)
    {
        one->load();
    }
}

void ScenarioManager::save()
{
    foreach (ScenarioItem* one, _itemList)
    {
        one->save();
    }
}

void ScenarioManager::saveAs(QString newName)
{
    foreach (ScenarioItem* one, _itemList)
    {
        one->saveAs(newName);
    }
}

void ScenarioManager::addItem(ScenarioItem* item)
{
    foreach (ScenarioItem* one, _itemList)
    {
        if (one == item)
        {
            _itemList.removeOne(one);
            return;
        }
    }
}
/*
bool ScenarioManager::contains(ScenarioItem* item)
{
    foreach (ScenarioItem* one, _itemList)
    {
        if (one == item)
        {
            return true;
        }
    }
}*/
