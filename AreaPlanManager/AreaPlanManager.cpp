#include "AreaPlanManager.h"
#include "AreaPlan.h"
#include <QCoreApplication>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QDebug>

AreaPlanManager::AreaPlanManager(QObject* parent)
{
    load(QCoreApplication::applicationDirPath() + "/Data/AreaPlan");
}

AreaPlanManager::~AreaPlanManager()
{
//	clear();
}

void AreaPlanManager::load(QString dirPath)
{
	QDir dir(dirPath);
	QStringList entryList = dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    foreach (QString str, entryList)
	{
		QString path = dir.path() + "/" + str;
		if (isPlanDir(path))
		{
			addItem(path);
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

void AreaPlanManager::addItem(QString planDir)
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
}

bool AreaPlanManager::has(QString name)
{
	if (nullptr == item(name))
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

AreaPlan* AreaPlanManager::item(QString name)
{
    foreach (AreaPlan* one, _itemList)
	{
		if (QString::compare(one->name(), name) == 0)
			return one;
	}
	return nullptr;
}

QQmlListProperty<AreaPlan> AreaPlanManager::itemList()
{
    return QQmlListProperty<AreaPlan>(this, _itemList);
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

