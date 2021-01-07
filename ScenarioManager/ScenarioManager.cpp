#include "ScenarioManager.h"
#include "ScenarioItem.h"
#include <SceneGraph/ISceneGraph.h>
#include <SceneGraph/IWindow.h>
#include <SceneGraph/IViewPort.h>
#include <GisMath/GisMath.h>
#include <QCoreApplication>
#include <QTextStream>
#include <QVector3D>
#include <QDebug>
#include <QDir>

ScenarioManager::ScenarioManager(QObject* parent):QObject(parent)
{
    _currentScenario    = nullptr;
    _sceneGraph         = nullptr;
    //setDir(QCoreApplication::applicationDirPath() + "/Data/Scenarios");
    //init();
}
/*
ScenarioManager::ScenarioManager(const ScenarioManager& other)
{
    qDebug() << "ScenarioManager::ScenarioManager(const ScenarioManager& other) exec";
    _dir = other.dir();
    _currentScenario = other.currentScenario();
    _scenarioList = other.scenarioList();
    _favoriteList = other.favoriteList();
    _itemList = other.itemList();
    _sceneGraph = other.sceneGraph();
}

void ScenarioManager::operator=(const ScenarioManager& other)
{
    qDebug() << "ScenarioManager::operator=(const ScenarioManager& other) exec";
    _dir = other.dir();
    _currentScenario = other.currentScenario();
    _scenarioList = other.scenarioList();
    _favoriteList = other.favoriteList();
    _itemList = other.itemList();
    _sceneGraph = other.sceneGraph();
}
*/
void ScenarioManager::setSceneGraph(ISceneGraph* sceneGraph)
{
    _sceneGraph = sceneGraph;
}

void ScenarioManager::init()
{
    clearScenarioList();
    clearFavoriteList();
    // 遍历子目录
    QStringList subDirList = _dir.entryList(QDir::NoDotAndDotDot | QDir::AllDirs);
    foreach (QString subDirName, subDirList)
    {
        QDir subDir(_dir.path() + "/" + subDirName);
        // 检查子目录是否是方案目录
        // ...
        // 创建只有名字的方案对象列表，而方案的具体数据仅在用户编辑、切换时加载
        Scenario* scenario = new Scenario();
        scenario->setScenarioManager(this);
        scenario->setName(subDirName);
        _scenarioList.append(scenario);        
        emit scenarioListChanged(scenarios());
    }
    // 加载收藏列表
    loadFavorites();
}

void ScenarioManager::clear()
{
    clearScenarioList();
    clearFavoriteList();
}

void ScenarioManager::setDir(QString dirPath)
{
    _dir = QDir(dirPath);
    init();
}

QDir ScenarioManager::dir() const
{
    return _dir;
}

Scenario* ScenarioManager::currentScenario() const
{
    return _currentScenario;
}

void ScenarioManager::setCurrentScenario(QString name)
{
    // 没有切换方案
    if (_currentScenario && QString::compare(name, _currentScenario->name()) == 0)
        return;
    // 方案名不存在
    if (! contains(name))
        return;
    // 切换当前方案
    _currentScenario = scenario(name);
    // 通知ScenarioItem切换目录（清理并重新加载）
    foreach (ScenarioItem* one, _itemList)
    {
        one->clear();
        one->load();
    }
}

Scenario* ScenarioManager::scenario(QString name)
{
    foreach (Scenario* one, _scenarioList)
    {
        if (QString::compare(one->name(), name) == 0)
            return one;
    }
    return nullptr;
}

QList<Scenario*> ScenarioManager::scenarioList() const
{
    return _scenarioList;
}

QList<Scenario*> ScenarioManager::favoriteList() const
{
    return _favoriteList;
}

QQmlListProperty<Scenario> ScenarioManager::scenarios()
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    return QQmlListProperty<Scenario>(this, _scenarioList);
#else
    return QQmlListProperty<Scenario>(this, &_scenarioList);
#endif
}

QQmlListProperty<Scenario> ScenarioManager::favorites()
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    return QQmlListProperty<Scenario>(this, _favoriteList);
#else
    return QQmlListProperty<Scenario>(this, &_favoriteList);
#endif
}

bool ScenarioManager::contains(QString name)
{
    foreach (Scenario* one, _scenarioList)
    {
        if (QString::compare(one->name(), name) == 0)
            return true;
    }
    return false;
}

int ScenarioManager::addScenario(QString name)
{
    // 已存在
    if (contains(name))
    {
        return -2;
    }
    // 创建目录失败
    if ( ! dir().mkdir(name))
    {
        return -3;
    }

    Scenario* scenario = new Scenario();
    scenario->setScenarioManager(this);
    scenario->setName(name);
    _scenarioList.append(scenario);
    _currentScenario = scenario;
    foreach (ScenarioItem* item, _itemList)
    {
        item->create();
    }
    emit scenarioListChanged(scenarios());
    return 1;
}

int ScenarioManager::removeScenario(QString name)
{
    removeFavorite(name);
    //
    Scenario* s = scenario(name);
    if(s == nullptr)
        return 0;
    // 移除目录失败
    //if(! dir().rmdir(name))
    //    return -3;
    QString path = dir().path() + "/" + name;
    QDir dir(path);
    dir.removeRecursively();
    _scenarioList.removeOne(s);
    delete s;
    emit scenarioListChanged(scenarios());
    return true;
}

int ScenarioManager::removeAllScenario()
{
    foreach (Scenario* one, _scenarioList)
    {
        if (one == nullptr)
            continue;
        if(! dir().rmdir(one->name()))
        {
            return -3;
        }
    }
    clearScenarioList();
    clearFavoriteList();
    saveFavorites();
    emit scenarioListChanged(scenarios());
}

int ScenarioManager::addFavorite(QString name)
{
    Scenario* s = scenario(name);
    if (s == nullptr)
        return 0;
    foreach (Scenario* one, _favoriteList)
    {
        if(one == s)    // 已在收藏中，不需重复添加
        {
            return -1;
        }
    }
    _favoriteList.append(s);
    saveFavorites();
    emit favoriteListChanged(favorites());
    return true;
}

int ScenarioManager::removeFavorite(QString name)
{
    Scenario* s = scenario(name);
    if (s == nullptr)
        return 0;
    if ( ! contains(name))
        return -1;
    _favoriteList.removeOne(s);
    saveFavorites();
    emit favoriteListChanged(favorites());
    return 1;
}

void ScenarioManager::load()
{
    // 基本信息（如名称、缩略图、描述等）
    if (_currentScenario)
    {
        _currentScenario->load();
    }

    // 定位
    locate();

    // 按模块（如演示动画、区域规划等）加载
    foreach (ScenarioItem* one, _itemList)
    {
        one->clear();
        one->setScenarioDir(_currentScenario->dir().path());
        one->load();
    }
}

void ScenarioManager::save()
{
    // 基本信息（如名称、缩略图、描述等）
    if (_currentScenario)
        _currentScenario->save();

    // 按模块（如演示动画、区域规划等）保存
    foreach (ScenarioItem* one, _itemList)
    {
        one->save();
    }
}

void ScenarioManager::saveAs(QString newName)
{
    if(contains(newName))
        return;
    if(addScenario(newName))
    foreach (ScenarioItem* one, _itemList)
    {
        one->save();
    }
}

QList<ScenarioItem*> ScenarioManager::itemList() const
{
    return _itemList;
}

void ScenarioManager::addItem(ScenarioItem* item)
{
    if (contains(item))
        return;
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

void ScenarioManager::locate()
{
    if(currentScenario() == nullptr)
        return;
    if(_sceneGraph == nullptr)
        return;
    QVector3D loc = currentScenario()->location();
    IViewPort* vp = _sceneGraph->GetMainWindow()->GetMainViewPoint();
    SceneViewPoint svp;
    // 无地图时使用xyz，有地图使用经纬高即可
    //double x,y,z;
    //GisMath::LBH2XYZ(loc.x(), loc.y(), loc.z(), x, y, z);
    //svp.stPos.fX = x;
    //svp.stPos.fY = y;
    //svp.stPos.fZ = z;
    svp.stPos.fX = loc.x();
    svp.stPos.fY = loc.y();
    svp.stPos.fZ = loc.z();
    svp.fElev = 45.0;
    //svp.fAzimuth = 89.0;
    svp.fDistance = 4000;
    vp->SetViewPoint(svp);
}

void ScenarioManager::loadFavorites()
{
    QFile file(_dir.path() + "/Favorites");
    if( file.open(QIODevice::Text | QIODevice::ReadOnly))
    {
        QString all = file.readAll();
        QStringList favoriteNameList = all.split(",");
        foreach (QString name, favoriteNameList)
        {
            Scenario* s = scenario(name);
            if(s)
            {
                _favoriteList.append(s);
            }
        }
        file.close();
    }
    else{

    }
}

void ScenarioManager::saveFavorites()
{
    QFile file(_dir.path() + "/Favorites");
    if( file.open(QIODevice::Text | QIODevice::WriteOnly))
    {
        QTextStream out(&file);
        for (int i = 0; i < _favoriteList.count(); i++)
        {
            if (_favoriteList[i] == nullptr)
                continue;
            out << _favoriteList[i]->name();
            if (i < _favoriteList.count() - 1)
                out << ",";
        }
        file.close();
    }
    else{

    }
}

void ScenarioManager::clearScenarioList()
{
    foreach(Scenario* one, _scenarioList)
    {
        if (one == nullptr)
            continue;
        delete one;
    }
    _scenarioList.clear();
    _currentScenario = nullptr;
}

void ScenarioManager::clearFavoriteList()
{
    _favoriteList.clear();
}

//ISceneGraph* ScenarioManager::sceneGraph() const
//{
//    return _sceneGraph;
//}
