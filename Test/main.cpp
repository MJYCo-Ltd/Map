#include <QApplication>
#include <QRandomGenerator>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include "../ScenarioManager/ScenarioManager.h"
#include "../ScenarioManager/Scenario.h"
#include "../AreaPlanManager/AreaPlanManager.h"
#include "../AreaPlanManager/AreaPlan.h"

static QString g_test_dirpath = "";
static QTextStream g_test_out;
static ScenarioManager g_mgr;

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

// -- 测试方案管理类 -------------------------------------------------
void testScenarioManager()
{
    g_mgr.setDir(g_test_dirpath + "/Scenarios");
    g_mgr.removeAllScenario();

    // 添加方案
    g_test_out << "-- test add new scenario --\n";
    for(int i = 0; i < 10; i++)
    {
        QString name = "scenario_" + QString::number(i);
        if ( g_mgr.addScenario(name) <= 0)
            g_test_out << "Failed to add scenario:" << name << "\n";
    }
    // 重复添加方案：名称已存在
    for(int i = 0; i < 2; i++)
    {
        QString name = "scenario_" + QString::number(i);
        if ( g_mgr.addScenario(name)  <= 0)
            g_test_out << "Failed to add scenario:" << name << "\n";
    }
    foreach(Scenario* one, g_mgr.scenarioList())
        g_test_out << one->name() << "\n";

    // 移除方案
    g_test_out << "-- test remove scenario --\n";
    for(int i = 7; i < 13; i++)
    {
        QString name = "scenario_" + QString::number(i);
        if (g_mgr.removeScenario(name) <= 0)
            g_test_out << "Failed to remove scenario:" << name << "\n";
    }
    foreach(Scenario* one, g_mgr.scenarioList())
        g_test_out << one->name() << "\n";

    // 添加收藏
    g_test_out << "-- test add favorite --\n";
    for(int i = 4; i < 8; i++)
    {
        QString name = "scenario_" + QString::number(i);
        if (g_mgr.addFavorite(name) <= 0)
            g_test_out << "Failed to add favorite:" << name << "\n";
    }
    foreach(Scenario* one, g_mgr.favoriteList())
        g_test_out << one->name() << "\n";

    // 移除收藏
    g_test_out << "-- test remove favorite --\n";
    for(int i = 5; i < 9; i++)
    {
        QString name = "scenario_" + QString::number(i);
        if (g_mgr.removeFavorite(name) <= 0)
            g_test_out << "Failed to remove favorite:" << name << "\n";
    }
    foreach(Scenario* one, g_mgr.favoriteList())
        g_test_out << one->name() << "\n";
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // -- 准备日志文件 ---------------------------------------------------
    g_test_dirpath = QApplication::applicationDirPath() + "/Test";

    QString testFilePath(g_test_dirpath + "/test.txt");
    QFile testFile(testFilePath);
    if ( ! testFile.open(QIODevice::Text | QIODevice::WriteOnly))
    {
        qDebug() << "Failed to Open File:" << testFile.fileName();
        return -1;
    }
    g_test_out.setDevice(&testFile);

    // -- 测试方案管理类 --
    testScenarioManager();

    // -- 关闭日志文件 ---------------------------------------------------
    testFile.close();

    // -- 比较文件,显示测试结果 --------------------------------------------
    QString stdAnswerFilePath(g_test_dirpath + "/StandardAnswer.txt");
    if(compare(testFilePath, stdAnswerFilePath))
    {
        qDebug() << "Succeed!";
        return 1;
    }
    else
    {
        qDebug() << "Failed!";
        return -1;
    }

    return a.exec();
}
