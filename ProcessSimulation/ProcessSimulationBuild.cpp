#include "ProcessSimulationBuild.h"
#include "CommandBuildComponent.h"
#include <QApplication>
#include <QDateTime>

ProcessSimulationBuild::ProcessSimulationBuild(QObject *parent) : QObject(parent)
{
    //sim.setStartDateTime(QDateTime(QDate(2020, 1, 1), QTime(0, 0, 0)));
    //sim.setEndDateTime(QDateTime(QDate(2021, 4, 30), QTime(0, 0, 0)));
    sim.setTimeRatio((2 * 24.0 * 60.0 * 60.0) / 1.0);   // 时间比率 2天/秒
    sim.setTimerInterval(100);

    QString dbFilePath = QApplication::applicationDirPath() + "/Data/Scenarios/Scenario.db";
    QString tableName = "ChangChun";
    data.init(dbFilePath, tableName);

    //for(int i = 0; i < 100; i++)
    //{
    //    QString name = "com_" + QString::number(i);
    //    QDateTime dt = QDateTime(QDate(2020,1,1),QTime(0,0,0)).addDays(i);
    //    int hc = 15 + i%20;
    //    double exp = 20 + i%10;
    //    CommandBuildComponent* component = new CommandBuildComponent();
    //    component->setName(name);
    //    component->setDateTime(dt);
    //    component->setHeadcount(hc);
    //    component->setExpense(exp);
    //    process.addComponent(component);
    //}
    process.addComponents(data.genCommandList());
    sim.addProcess(&process);
    //sim.start();
}

void ProcessSimulationBuild::start()
{
    sim.start();
}
