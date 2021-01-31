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

    bool bChecked = CheckPC(argv);
    if(!bChecked)
    {
        return(-1);
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
