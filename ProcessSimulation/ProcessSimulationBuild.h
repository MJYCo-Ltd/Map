#ifndef PROCESSSIMULATIONBUILD_H
#define PROCESSSIMULATIONBUILD_H
/*************************************************
* Copyright(C)
* File name:    ProcessSimulationBuild
* Author:       魏晓亮
* Version:      1.1
* Date:         2021/1/4
* Description:  施工进度模拟外部接口
* History:
*************************************************/

#include "ProcessSimulation_global.h"
#include "ProcessBuildData.h"
#include "ProcessBuild.h"
#include "Simulation.h"
#include <QObject>

class PROCESSSIMULATION_EXPORT ProcessSimulationBuild : public QObject
{
    Q_OBJECT
public:
    explicit ProcessSimulationBuild(QObject *parent = nullptr);

    Q_INVOKABLE void start();
    //Q_INVOKABLE void pause();
    //Q_INVOKABLE void resume();
    //Q_INVOKABLE void stop();
    //Q_INVOKABLE void pauseOrResume();                               // switch state (by pushing button) repeatedly
    //Q_INVOKABLE void goTo(QDateTime& dt);
signals:

public slots:

private:
    Simulation                  sim;
    ProcessBuildData            data;
    ProcessBuild                process;
};

#endif // PROCESSSIMULATIONBUILD_H
