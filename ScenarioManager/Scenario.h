#ifndef SCENARIO_H
#define SCENARIO_H

#include "ScenarioManager_global.h"
#include <QObject>
#include <QString>
#include <QDir>

class ScenarioManager;
class SCENARIOMANAGER_EXPORT Scenario : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString imageFilePath READ imageFilePath NOTIFY imageFilePathChanged)
public:
    Scenario(QObject* parent = nullptr);

    QDir dir();
    QString name();
    void setName(QString);
    QString imageFilePath();
signals:
    void nameChanged();
    void imageFilePathChanged();
protected:
    ScenarioManager*    _mgr;
    QString             _name;
};

#endif // SCENARIO_H
