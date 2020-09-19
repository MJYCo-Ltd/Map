#include "ScenarioManager.h"
#include "ScenarioItem.h"
#include "Scenario.h"
#include <QCoreApplication>
#include <QTextStream>
#include <QDebug>
#include <QFile>

ScenarioManager::ScenarioManager(QObject* parent)
{
    //addTempScenario();

    init();
}

ScenarioManager::~ScenarioManager()
{
    QDir dir = this->dir();
    // read favorite list
    QFile file(dir.path() + "/favorites");
    if ( ! file.open(QFile::WriteOnly | QFile::Text) )
        qDebug() << "file : " << file.fileName() << " open failed!";
    //QString text = file.readAll();
    //QStringList list = text.split(",");
    QTextStream out(&file);
    for(int i = 0; i < _favoriteList.count(); i++)
    {
        if (i > 0)
            out << ",";
        out << _favoriteList[i]->name();
    }
    file.close();
}

void ScenarioManager::init()
{
    QString sPath = QCoreApplication::applicationDirPath() + "/Data/Scenarios";
    setDir(sPath);
    // read scenario list
    QDir dir = this->dir();
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList names = dir.entryList();
    foreach(QString name, names)
    {
        Scenario* snr = new Scenario(this);
        snr->setName(name);
        if (snr)
            _scenarioList.append(snr);
    }
    // read favorite list
    QFile file(dir.path() + "/favorites");
    if ( ! file.open(QFile::ReadOnly | QFile::Text) )
        qDebug() << "file : " << file.fileName() << " open failed!";
    QString text = file.readAll();
    QStringList list = text.split(",");
    foreach(QString one, list)
    {
        Scenario* s = scenario(one);
        if(s != nullptr)
        {
            _favoriteList.append(s);
        }
    }
    file.close();
}

QDir ScenarioManager::dir()
{
    return _dir;
}

void ScenarioManager::setDir(QDir dir)
{
    _dir = dir;
}

QDir ScenarioManager::scenarioDir()
{
    return _currentScenario->dir();
}

Scenario* ScenarioManager::currentScenario()
{
    return _currentScenario;
}

void ScenarioManager::setCurrentScenario(QString name)
{
    foreach(Scenario* one, _scenarioList)
    {
        if (0) // QString(one->name(), name)
        {
            _currentScenario = one;
            return;
        }
    }
}

//QList<Scenario*> ScenarioManager::scenarios()
//{
//    return _scenarioList;
//}
QQmlListProperty<Scenario> ScenarioManager::scenarios()
{
    return QQmlListProperty<Scenario>(this, _scenarioList);

}

QQmlListProperty<Scenario> ScenarioManager::favorites()
{
    return QQmlListProperty<Scenario>(this, _favoriteList);
}

QStringList ScenarioManager::scenarioNames()
{
    //QDir dir = this->dir();
    //dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    //return dir.entryList();
    QStringList names;
    foreach(Scenario* one, _scenarioList)
    {
        names.append(one->name());
    }
    return names;
}

QStringList ScenarioManager::favoriteNames()
{
    QStringList names;
    foreach(Scenario* one, _favoriteList)
    {
        names.append(one->name());
    }
    return names;
}

bool ScenarioManager::contains(QString name)
{
    return (scenario(name) != nullptr);
}

Scenario* ScenarioManager::scenario(QString name)
{
    foreach(Scenario* one, _scenarioList)
    {
        if (QString::compare(name, one->name()) == 0)
        {
            return one;
        }
    }
    return nullptr;
}

void ScenarioManager::addFavorite(QString name)
{
    addFavorite(scenario(name));
}

void ScenarioManager::addFavorite(Scenario* scenario)
{
    if (scenario == nullptr)
        return;
    foreach(Scenario* one, _favoriteList)
    {
        if (one == scenario)
            return;
    }
    _favoriteList.append(scenario);
    emit favoriteListChanged();
}

void ScenarioManager::removeFavorite(QString name)
{
    removeFavorite(scenario(name));
}

void ScenarioManager::removeFavorite(Scenario* scenario)
{
    if (scenario == nullptr)
        return;
    _favoriteList.removeOne(scenario);
    emit favoriteListChanged();
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

bool ScenarioManager::addScenario(QString name, QString fp)
{
    if (contains(name))
        return false;
    if ( ! dir().mkdir(name))
        return false;
    Scenario* scenario = new Scenario(this);
    if (scenario == nullptr)
        return false;
    scenario->setName(name);
    // copy image file
    if(fp.endsWith(".png"))
        QFile::copy(fp, scenario->imageFilePath());
    _scenarioList.append(scenario);
    emit scenarioListChanged();
    return true;
}

void ScenarioManager::removeScenario(QString name)
{
    _scenarioList.removeOne(scenario(name));
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
/*
void ScenarioManager::saveAs(QString newName)
{
    foreach (ScenarioItem* one, _itemList)
    {
        one->saveAs(newName);
    }
}*/

void ScenarioManager::addItem(ScenarioItem* item)
{
    if( ! contains(item))
        _itemList.append(item);
}

bool ScenarioManager::contains(ScenarioItem* item)
{
    foreach (ScenarioItem* one, _itemList)
    {
        if (one == item)
        {
            return true;
        }
    }
    return false;
}
