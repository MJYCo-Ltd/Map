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

void AreaPlanManager::setCurrentLayer(QString layerName)
{
    if (currentPlan() == nullptr)
        return;
    currentPlan()->setCurrentLayer(layerName);
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

