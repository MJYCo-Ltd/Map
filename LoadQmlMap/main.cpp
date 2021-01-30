#include <iostream>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <SatelliteToolKit/SatelliteToolKit.h>
#include "Qml/QtOsgItem.h"
#include "ISceneCore.h"
#include "AppGlobal.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGLRhi);
#endif

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

    std::string sErrorInfo;
    if(!InitSatelliteToolKit(GetExePath(),sErrorInfo))
    {
        std::cout<<sErrorInfo<<std::endl;
    }

    qmlRegisterType<QAppGlobal>("MyItem",1,0,"AppGlobal");
    qmlRegisterType<QtOsgItem>("MyItem",1,0,"OsgItem");

    QQmlApplicationEngine engine;

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    const QUrl url(QStringLiteral("qrc:/main.qml"));
#else
    const QUrl url(QStringLiteral("qrc:/main6.qml"));
#endif
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
