#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "Qml/QtOsgItem.h"
#include "ISceneCore.h"
#include "AppGlobal.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    bool bChecked = CheckPC();
    if(!bChecked)
    {
        return(-1);
    }

    char* pstr = strrchr(argv[0],'\\');
    pstr[0] = 0;
    SetExePath(argv[0]);


    qmlRegisterType<QAppGlobal>("MyItem",1,0,"AppGlobal");
    qmlRegisterType<QtOsgItem>("MyItem",1,0,"OsgItem");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
