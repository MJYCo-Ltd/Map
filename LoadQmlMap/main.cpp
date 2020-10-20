#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "Qml/QtOsgItem.h"
#include "ISceneCore.h"
#include "AppGlobal.h"

//#include "../ScenarioManager/Scenario.h"
#include "../ScenarioManager/ScenarioManager.h"
#include "../AreaPlanManager/AreaPlan.h"
#include "../AreaPlanManager/AreaPlanManager.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    bool bChecked = CheckPC();
    if(!bChecked)
    {
        return(-1);
    }

#ifdef Q_OS_WIN
    char * pStr = strrchr(argv[0],'\\');
#else
    char * pStr = strrchr(argv[0],'/');
#endif
    pStr[0] = 0;
    SetExePath(argv[0]);

    qmlRegisterType<QAppGlobal>("MyItem",1,0,"AppGlobal");
    qmlRegisterType<QtOsgItem>("MyItem",1,0,"OsgItem");
    qmlRegisterType<ScenarioManager>("MyItem",1,0,"ScenarioManager");
    qmlRegisterType<AreaPlanManager>("MyItem",1,0,"AreaPlanManager");
//    qmlRegisterType<Scenario>("MyItem",1,0,"Scenario");
    qmlRegisterType<AreaPlan>("MyItem",1,0,"AreaPlan");

    QQmlApplicationEngine engine;

    //QString sPath = QCoreApplication::applicationDirPath() + "/Data/Scenarios";
    //ScenarioManager scenarioManager();
    //engine.rootContext()->setContextProperty("scenarioManager", &scenarioManager);
    //AreaPlanManager areaPlanManager();
    //engine.rootContext()->setContextProperty("areaPlanManager", &areaPlanManager);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
