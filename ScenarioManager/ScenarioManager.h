#ifndef SCENARIOMANAGER_H
#define SCENARIOMANAGER_H

#include "ScenarioManager_global.h"
#include <QObject>
#include <QStringList>
#include <QString>
#include <QList>
#include <QDir>

class Scenario;
class ScenarioItem;
class SCENARIOMANAGER_EXPORT ScenarioManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList scenarioNames READ scenarioNames NOTIFY scenarioListChanged)
    Q_PROPERTY(QStringList favoriteNames READ favoriteNames NOTIFY favoriteListChanged)
public:
    ScenarioManager(QDir dir);
    QDir dir();
    QDir scenarioDir();

    // scenario
    Scenario* currentScenario();
    void setCurrentScenario(QString name);
    QStringList scenarioNames();
    QStringList favoriteNames();
    bool addScenario(QString name, QString imageFilePath = "");
    void addTempScenario(); // example : untitled_1
    Scenario* scenario(QString name);
    void addFavorite(QString name);
    void addFavorite(Scenario*);
    bool contains(QString name);
    void removeScenario(QString name);
    void load();
    void save();
    //void saveAs(QString newName);

    // scenario item
    void addItem(ScenarioItem*);
    bool contains(ScenarioItem*);
signals:
    void scenarioListChanged();
    void favoriteListChanged();
protected:
    void init();
    QList<Scenario*> scenarios();
protected:
    QDir                    _dir;
    QDir                    _scenarioDir;
    //QString                 _currentScenario;
    Scenario*               _currentScenario;
    QList<Scenario*>        _favoriteList;
    QList<Scenario*>        _scenarioList;
    //QList<QString>          _favoriteNameList;
    //QList<QString>          _scenarioNameList;
    QList<ScenarioItem*>	_itemList;
};

#endif // SCENARIOMANAGER_H
