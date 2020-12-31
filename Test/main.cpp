#include <QCoreApplication>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include "../ScenarioManager/ScenarioManager.h"
#include "../ScenarioManager/Scenario.h"

bool compare(QString fp1, QString fp2)
{
    QFile f1(fp1);
    if ( ! f1.open(QIODevice::Text | QIODevice::ReadOnly))
    {
        qDebug() << "Failed to Open File:" << f1.fileName();
        return false;
    }
    QFile f2(fp2);
    if ( ! f2.open(QIODevice::Text | QIODevice::ReadOnly))
    {
        qDebug() << "Failed to Open File:" << f2.fileName();
        return false;
    }

    bool res = true;
    while(!f1.atEnd())
    {
        if(f2.atEnd())
        {
            res = false;
        }
        QString s1 = f1.readLine();
        QString s2 = f2.readLine();
        if(QString::compare(s1,s2) == 0)
        {
            continue;
        }
        else
        {
            res = false;
            break;
        }
    }
    if(!f2.atEnd())
        res = false;

    f1.close();
    f2.close();
    return res;
}

int testScenarioManager()
{
    // -- 测试方案管理类 -------------------------------------------------
    ScenarioManager mgr;
    mgr.setDir(QCoreApplication::applicationDirPath() + "/Test/Scenarios");
    mgr.init();
    mgr.removeAllScenario();
    QDir dir = mgr.dir();
    QFile testFile(dir.path() + "/test.txt");
    if ( ! testFile.open(QIODevice::Text | QIODevice::WriteOnly))
    {
        qDebug() << "Failed to Open File:" << testFile.fileName();
        return -1;
    }
    QTextStream out(&testFile);

    // 添加方案
    out << "-- test add new scenario --\n";
    for(int i = 0; i < 10; i++)
    {
        QString name = "scenario_" + QString::number(i);
        if ( mgr.addScenario(name) <= 0)
            out << "Failed to add scenario:" << name << "\n";
    }
    // 重复添加方案：名称已存在
    for(int i = 0; i < 2; i++)
    {
        QString name = "scenario_" + QString::number(i);
        if ( mgr.addScenario(name)  <= 0)
            out << "Failed to add scenario:" << name << "\n";
    }
    foreach(Scenario* one, mgr.scenarioList())
        out << one->name() << "\n";

    // 移除方案
    out << "-- test remove scenario --\n";
    for(int i = 7; i < 13; i++)
    {
        QString name = "scenario_" + QString::number(i);
        if (mgr.removeScenario(name) <= 0)
            out << "Failed to remove scenario:" << name << "\n";
    }
    foreach(Scenario* one, mgr.scenarioList())
        out << one->name() << "\n";

    // 添加收藏
    out << "-- test add favorite --\n";
    for(int i = 4; i < 8; i++)
    {
        QString name = "scenario_" + QString::number(i);
        if (mgr.addFavorite(name) <= 0)
            out << "Failed to add favorite:" << name << "\n";
    }
    foreach(Scenario* one, mgr.favoriteList())
        out << one->name() << "\n";

    // 移除收藏
    out << "-- test remove favorite --\n";
    for(int i = 5; i < 9; i++)
    {
        QString name = "scenario_" + QString::number(i);
        if (mgr.removeFavorite(name) <= 0)
            out << "Failed to remove favorite:" << name << "\n";
    }
    foreach(Scenario* one, mgr.favoriteList())
        out << one->name() << "\n";

    mgr.scenarios();
    testFile.close();

    // -- 比较文件,显示测试结果 --------------------------------------------
    if(compare(dir.path() + "/test.txt", dir.path() + "/StandardAnswer.txt"))
    {
        qDebug() << "Succeed!";
        return 1;
    }
    else
    {
        qDebug() << "Failed!";
        return -1;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // -- 测试方案管理类 --
    testScenarioManager();

    return a.exec();
}
