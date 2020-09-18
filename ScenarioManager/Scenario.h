#ifndef SCENARIO_H
#define SCENARIO_H

#include <QObject>
#include <QString>
#include <QDir>

class ScenarioManager;
class Scenario : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString imageFilePath READ imageFilePath)
public:
    Scenario(ScenarioManager* mgr, QString name);

    QDir dir();
    QString name();
    QString imageFilePath();
private:
    ScenarioManager*    _mgr;
    QString             _name;
};

#endif // SCENARIO_H
