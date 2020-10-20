#ifndef SCENARIOMANAGER_H
#define SCENARIOMANAGER_H

#include "ScenarioManager_global.h"
#include <QObject>
#include <QStringList>
#include <QString>
#include <QList>
#include <QDir>

class ScenarioItem;
class SCENARIOMANAGER_EXPORT ScenarioManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList scenarios READ scenarios NOTIFY scenarioListChanged)
public:
    ScenarioManager(QDir dir);
    QDir dir();
    QDir scenarioDir();

    QString currentScenario();
    void setCurrentScenario(QString name);
    QStringList scenarios();

    bool contains(QString name);
    bool addScenario(QString name);
    void addTempScenario(); // example : untitled_1
    void removeScenario(QString name);
    void load();
    void save();
    void saveAs(QString newName);

    // scenario item
    Q_INVOKABLE void addItem(ScenarioItem*);
    bool contains(ScenarioItem*);

    void addItem(ScenarioItem*);
    //void removeScenarioItem(ScenarioItem*);
    //bool contains(ScenarioItem*);

signals:
    void scenarioListChanged();
protected:
    QDir                    _dir;
    QDir                    _scenarioDir;
    QString                 _currentScenario;
    QList<ScenarioItem*>	_itemList;
};

#endif // SCENARIOMANAGER_H
