#ifndef SCENARIOITEM_H
#define SCENARIOITEM_H
/*
 * a ScenarioItem is a module that can be loaded/saved
 */
#include "ScenarioManager_global.h"
#include <QString>
#include <QList>
#include <QDir>

class ScenarioManager;
class SCENARIOMANAGER_EXPORT ScenarioItem
{
public:
    ScenarioItem(const QString name, ScenarioManager* mgr);
    const QString name();
    void saveAs(const QString name);    // also called rename

    virtual void load() = 0;
    virtual void save() = 0;

protected:
    QDir dir();

protected:
    QString                 _name;
    ScenarioManager*        _mgr;
};

#endif // SCENARIOITEM_H
