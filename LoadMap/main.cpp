#include "mainwindow.h"
#include <QApplication>
#include <QWindow>
#include <QDebug>
#include <ISceneCore.h>
#include <SceneGraph/IWindow.h>
#include <SpaceEnv/ISpaceEnv.h>
#include <Map/IMap.h>


/**
 * @brief 窗口消息定义
 */
class WindowMessage:public IWindowMessageObserver
{
public:
    void KeyUp(char chr){qDebug()<<(int)chr;}
    void MouseDown(MouseButtonMask type, int, int)
    {
        switch (type)
        {
        case LEFT_MOUSE_BUTTON:
            qDebug()<<"Left Mouse";
            break;
        case RIGHT_MOUSE_BUTTON:
            qDebug()<<"right Mouse";
            break;
        case MIDDLE_MOUSE_BUTTON:
            qDebug()<<"Middle Mouse";
            break;
        }
        qDebug()<<"Mouse down";
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    bool bChecked = CheckPC();

    if(!bChecked)
    {
        return(1);
    }
#ifdef Q_OS_WIN
    char * pStr = strrchr(argv[0],'\\');
#else
    char * pStr = strrchr(argv[0],'/');
#endif
    pStr[0] = 0;
    SetExePath(argv[0]);

    MainWindow w;
    w.show();



    auto pScene = GetSceneCore()->GetSceneGraphManager()->CreateSceneGraph(SCENEGRAPH_3D);
//    pScene->GetMap()->GetSpaceEnv()->SetMilkywayVisible(false);
//    for(auto one:pScene->GetMap()->GetMapLayers())
//    {
//        qDebug()<<*one.c_str();
//    }

    w.SetSecenGraph(pScene);

    //pScene->GetMainWindow()->SubMessage(new WindowMessage);

    QWidget* pWindow = pScene->GetMainWindow()->ToWidget();
    if(nullptr != pWindow)
    {
        w.setCentralWidget(pWindow);
    }

    pScene->Run();


    return a.exec();
}
