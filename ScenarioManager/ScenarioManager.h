#ifndef SCENARIOMANAGER_H
#define SCENARIOMANAGER_H

#include "ScenarioManager_global.h"
#include "Scenario.h"
#include <QObject>
#include <QQmlListProperty>
#include <QStringList>
#include <QString>
#include <QList>
#include <QDir>

Q_DECLARE_METATYPE(QQmlListProperty<Scenario>)
//class Scenario;
class ScenarioItem;
class SCENARIOMANAGER_EXPORT ScenarioManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QDir dir READ dir WRITE setDir)
    //Q_PROPERTY(QStringList scenarioNames READ scenarioNames NOTIFY scenarioListChanged)
    //Q_PROPERTY(QStringList favoriteNames READ favoriteNames NOTIFY favoriteListChanged)
    Q_PROPERTY(QQmlListProperty<Scenario> scenarios READ scenarios NOTIFY scenarioListChanged)
    Q_PROPERTY(QQmlListProperty<Scenario> favorites READ favorites NOTIFY favoriteListChanged)
public:
    ScenarioManager(QObject* parent = nullptr);
    ~ScenarioManager();
    void init();

    QDir dir();
    void setDir(QDir);
    QDir scenarioDir();
    // scenario
    Scenario* currentScenario();
    Q_INVOKABLE void setCurrentScenario(QString name);
    QStringList scenarioNames();
    QStringList favoriteNames();
    QQmlListProperty<Scenario> scenarios();
    QQmlListProperty<Scenario> favorites();
    bool addScenario(QString name, QString imageFilePath = "");
    void addTempScenario(); // example : untitled_1
    Scenario* scenario(QString name);
    Q_INVOKABLE void addFavorite(QString name);
    Q_INVOKABLE void addFavorite(Scenario*);
    Q_INVOKABLE void removeFavorite(QString name);
    Q_INVOKABLE void removeFavorite(Scenario*);
    Q_INVOKABLE void removeScenario(QString name);
    bool contains(QString name);
    Q_INVOKABLE void load();
    void save();
    //void saveAs(QString newName);

    // scenario item
    Q_INVOKABLE void addItem(ScenarioItem*);
    bool contains(ScenarioItem*);
signals:
    void scenarioListChanged();
    void favoriteListChanged();

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
