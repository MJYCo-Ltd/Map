#include "mainwindow.h"
#include <QApplication>
#include <QWindow>
#include <QDebug>
#include "ISceneCore.h"

/**
 * @brief 窗口消息定义
 */
class WindowMessage:public IWindowMessageObserver
{
public:
    void KeyUp(char chr){qDebug()<<chr;}
    void MouseDown(MouseButtonMask, int, int){qDebug()<<"Mouse down";}
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
#elif
    char * pStr = strrchr(argv[0],'/');
#endif
    pStr[0] = 0;
    SetExePath(argv[0]);

    MainWindow w;
    w.show();



    auto pScene = GetSceneCore()->GetSceneGraphManager()->CreateSceneGraph(SCENEGRAPH_3D,"");

    w.SetSecenGraph(pScene);

//    pScene->GetMainWindow()->SubMessage(new WindowMessage);

    QWidget* pWindow = pScene->GetMainWindow()->ToWidget();
    if(nullptr != pWindow)
    {
        w.setCentralWidget(pWindow);
    }

    pScene->Run();


    return a.exec();
}
