#include "AreaPlanManager.h"
#include "AreaPolygonEditor.h"
#include "AreaPlan.h"
#include <QCoreApplication>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QDebug>

AreaPlanManager::AreaPlanManager(QObject* parent):QObject(parent)
{
    _currentPlan = nullptr;
    setName("AreaPlan");

    connect(this, SIGNAL(currentPlanChanged()), this, SIGNAL(areaListChanged()));
    connect(this, SIGNAL(currentLayerChanged()), this, SIGNAL(areaListChanged()));
}

AreaPlanManager::~AreaPlanManager()
{
    clear();
}

void AreaPlanManager::setSceneGraph(ISceneGraph* sceneGraph)
{
    AreaPolygonEditor* editor = AreaPolygonEditor::getInstance();
    editor->setSceneGraph(sceneGraph);
    connect(editor, SIGNAL(addArea(AreaPolygon*)), this, SLOT(onAddArea(AreaPolygon*)));
}

void AreaPlanManager::load()
{
    QStringList entryList = dir().entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    foreach (QString str, entryList)
    {
        QString path = dir().path() + "/" + str;
        if (isPlanDir(path))
        {
            addPlan(path);
        }
    }
    if(planList().count() > 0)
        setCurrentPlan(0);
}

void AreaPlanManager::save()
{
    foreach (AreaPlan* one, _itemList)
	{
		one->save();
	}
}

void AreaPlanManager::addPlan(QString planDir)
{
	QDir dirPlan(planDir);
	if (!dirPlan.exists())
		return;
	if (!has(dirPlan.dirName()))
	{
        AreaPlan* plan = new AreaPlan();
        if (plan)
        {
            plan->setName(dirPlan.dirName());
            plan->load(planDir);
            _itemList.append(plan);
            emit planListChanged();
            //qDebug() << "add plan:" << plan->name();
        }
	}	
}

void AreaPlanManager::clear()
{
    foreach (AreaPlan* one, _itemList)
	{
		if (one)
		{
			delete one;
			one = nullptr;
		}
	}
    _itemList.clear();

    emit planListChanged();
}

bool AreaPlanManager::has(QString name)
{
        if (nullptr == plan(name))
		return false;
	else
		return true;
}

bool AreaPlanManager::has(AreaPlan* plan)
{
	bool res = false;
    foreach (AreaPlan* one, _itemList)
	{
		if (one == plan)
			res = true;
	}
	return res;
}

AreaPlan* AreaPlanManager::plan(int index)
{
    if (planList().count() > index)
        return planList()[index];
    else
        return nullptr;
}

AreaPlan* AreaPlanManager::plan(QString name)
{
    foreach (AreaPlan* one, _itemList)
	{
		if (QString::compare(one->name(), name) == 0)
			return one;
	}
	return nullptr;
}

QList<AreaPlan*> AreaPlanManager::planList()
{
    return _itemList;
}

QStringList AreaPlanManager::planNames()
{
    QStringList list;
    for(int i = 0; i < planList().count(); i++)
    {
        list.append(planList()[i]->name());
    }
    return list;
}

QQmlListProperty<AreaPlan> AreaPlanManager::plans()
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    return QQmlListProperty<AreaPlan>(this, _itemList);
#else
    return QQmlListProperty<AreaPlan>(this, &_itemList);
#endif
}

QQmlListProperty<AreaPlanLayer> AreaPlanManager::layers()
{
    if (currentPlan())
    {
        _layerList = currentPlan()->layerList();
        //qDebug() << "LAYER LIST COUNT:" << _layerList.count();
    }
    else
    {
        _layerList.clear();
        //qDebug() << "CURRENT PLAN IS NULL!";
    }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    return QQmlListProperty<AreaPlanLayer>(this, _layerList);
#else
    return QQmlListProperty<AreaPlanLayer>(this, &_layerList);
#endif
}
/*
QQmlListProperty<AreaPolygon> AreaPlanManager::areas()
{
    qDebug() << "AreaPlanManager::areas exec";
    if (currentPlan() && currentPlan()->currentLayer())
    {
        _polygonList = currentPlan()->currentLayer()->areaList();
        qDebug() << "_polygonList.count()" << _polygonList.count();
    }
    else
    {
        _polygonList.clear();
        qDebug() << "_polygonList.clear()";
    }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    return QQmlListProperty<AreaPolygon>(this, _polygonList);
#else
    return QQmlListProperty<AreaPolygon>(this, &_polygonList);
#endif
}*/

int AreaPlanManager::areaCount()
{
    if (currentPlan() && currentPlan()->currentLayer())
    {
        return currentLayer()->areaList().count();
    }
    else
    {
        return 0;
        qDebug() << "_polygonList.clear()";
    }
}

bool AreaPlanManager::isCurrentPlan(QString planName)
{
    //qDebug() << "AreaPlanManager::isCurrentPlan exec";
    if (currentPlan() == nullptr)
    {
        //qDebug() << "currentPlan() == nullptr";
        return false;
    }
    else
    {
        //qDebug() << "currentPlan()->name():" << currentPlan()->name();
        return (QString::compare(currentPlan()->name(), planName) == 0);
    }
}

AreaPlan* AreaPlanManager::currentPlan()
{
    return _currentPlan;
}

void AreaPlanManager::setCurrentPlan(AreaPlan* newPlan)
{
    if (newPlan == nullptr)
        return;
    if (_currentPlan &&
            (_currentPlan->name() == newPlan->name()))
        return;
    else
    {
        if (_currentPlan)
        {
            _currentPlan->setVisible(false);
            disconnect(_currentPlan,SIGNAL(layerListChanged()), this, SIGNAL(layerListChanged()));
        }
        _currentPlan = newPlan;
        _currentPlan->setVisible(true);
        connect(_currentPlan,SIGNAL(layerListChanged()), this, SIGNAL(layerListChanged()));

        emit currentPlanChanged();
    }
}

void AreaPlanManager::setCurrentPlan(int index)
{
    setCurrentPlan(plan(index));
}

void AreaPlanManager::setCurrentPlan(QString name)
{
    setCurrentPlan(plan(name));
}

AreaPlanLayer* AreaPlanManager::currentLayer()
{
    if (currentPlan() == nullptr)
        return nullptr;
    else
        return currentPlan()->currentLayer();
}

void AreaPlanManager::setCurrentLayer(AreaPlanLayer* newLayer)
{
    if (newLayer == nullptr)
        return;
    if (currentPlan() == nullptr)
        return;
    if (currentPlan()->currentLayer()
            && currentPlan()->currentLayer()->name() == newLayer->name())
        return;
    else{
        if (currentPlan()->currentLayer())
        {
            disconnect(currentPlan()->currentLayer(), SIGNAL(areaListChanged()), this, SIGNAL(areaListChanged()));
        }
        currentPlan()->setCurrentLayer(newLayer->name());
        connect( currentPlan()->currentLayer(), SIGNAL(areaListChanged()), this, SIGNAL(areaListChanged()));
        emit currentLayerChanged();
        emit areaListChanged();
    }
}

void AreaPlanManager::setCurrentLayer(int index)
{
    if (currentPlan() == nullptr)
        return;
    setCurrentLayer(currentPlan()->layer(index));
}

void AreaPlanManager::setCurrentLayer(QString layerName)
{
    if (currentPlan() == nullptr)
        return;
    setCurrentLayer(currentPlan()->layer(layerName));
}

QColor AreaPlanManager::currentLayerColor()
{
    if (currentPlan() == nullptr)
        return QColor("transparent");
    if (currentPlan()->currentLayer() == nullptr)
        return QColor("transparent");
    return currentPlan()->currentLayer()->color();
}

void AreaPlanManager::setEditMode(bool editMode)
{
    AreaPolygonEditor::getInstance()->setEnable(editMode);
    if (editMode)
        emit startEditMode();
}

void AreaPlanManager::startEdit()
{
    // 完成区域编辑界面QML后解开此注释
    if (currentPlan() == nullptr)
        return;
    AreaPlanLayer* cl = currentPlan()->currentLayer();
    if (cl == nullptr)
        return;
    SceneColor c;
    c.fR = cl->color().red() / 255.0;
    c.fG = cl->color().green() / 255.0;
    c.fB = cl->color().blue() / 255.0;
    c.fA = cl->color().alpha() / 255.0;
    AreaPolygonEditor::getInstance()->setColor(c);

    //AreaPolygonEditor::getInstance()->start();
}

void AreaPlanManager::locateArea(int index)
{
    AreaPolygonEditor::getInstance()->locatePolygon(area(index));
}

void AreaPlanManager::locateArea(AreaPolygon* polygon)
{
    AreaPolygonEditor::getInstance()->locatePolygon(polygon);
}

void AreaPlanManager::removeArea(int index)
{
    if (currentLayer() == nullptr)
        return;
    currentLayer()->removeAreaPlanPolygon(index);
}

void AreaPlanManager::removeArea(AreaPolygon* polygon)
{
    if (currentLayer() == nullptr)
        return;
    currentLayer()->removeAreaPlanPolygon(polygon);
}

void AreaPlanManager::setAreaVisible(int index, bool v)
{
    AreaPolygon* polygon = area(index);
    if (polygon)
        polygon->setVisible(v);
}

void AreaPlanManager::setLayerVisible(int index, bool v)
{
    AreaPlanLayer* l = layer(index);
    if (l)
        l->setVisible(v);
}

void AreaPlanManager::onAddArea(AreaPolygon* ap)
{
    if (_currentPlan == nullptr)
        return;
    AreaPlanLayer* cl = _currentPlan->currentLayer();
    if (cl == nullptr)
        return;
    cl->addAreaPolygon(ap);
}

bool AreaPlanManager::isPlanDir(QString dirPath)
{
	QFileInfo fileInfo(dirPath);
	if (!fileInfo.isDir())
		return false;
	QDir dir(dirPath);
	// plan dir should has (at least) one json file
	QStringList entryList = dir.entryList(QStringList("*.json"), QDir::Files);
	if (entryList.count() > 0)
		return true;
	else
		return false;
}

AreaPolygon* AreaPlanManager::area(int index)
{
    if (index < 0)
        return nullptr;
    if (currentPlan() == nullptr)
        return nullptr;
    if (currentPlan()->currentLayer() == nullptr)
        return nullptr;
    if (currentPlan()->currentLayer()->areaList().count() < index)
        return nullptr;
    return currentPlan()->currentLayer()->areaList()[index];
}

AreaPlanLayer* AreaPlanManager::layer(int index)
{
    if (index < 0)
        return nullptr;
    if (currentPlan() == nullptr)
        return nullptr;
    if (currentPlan()->layerList().count() < index)
        return nullptr;
    return currentPlan()->layerList()[index];
}
