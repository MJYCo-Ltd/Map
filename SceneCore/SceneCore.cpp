#include <QtCore>
#include <osgEarth/Registry>
#include <osgEarth/Capabilities>
#include <osgDB/ReadFile>
#include "MyNotify.h"
#include "SceneCore.h"
#include "SceneGraph/SceneGraphManager.h"
static CSceneCore* s_gMapCore=nullptr;
static int g_num;
static bool        s_gBChecked(false);
static string     s_strPath;

void my_init()
{
    ++g_num;
    osg::notify(osg::NOTICE)<<"my_init scenecore"<<g_num;
}

void my_fini()
{
    --g_num;
    osg::notify(osg::NOTICE)<<"my_fini scenecore"<<g_num;
    if(0==g_num)
    {
        delete s_gMapCore;
    }
}

#ifdef Q_OS_WIN
#include <qt_windows.h>
#undef LoadImage

BOOL WINAPI DllMain(
  _In_ HINSTANCE hinstDLL, /// 指向自身的句柄
  _In_ DWORD  fdwReason,   /// 调用原因
  _In_ LPVOID lpvReserved  /// 隐式加载和显式加载
)
{
    switch(fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        my_init();
        break;
    case DLL_PROCESS_DETACH:
        my_fini();
        break;
//    case DLL_THREAD_ATTACH:
//        my_init();
//        break;
//    case DLL_THREAD_DETACH:
//        my_fini();
//        break;
    }

    return(TRUE);
}
#else
void my_init(void) __attribute__((constructor)); //告诉gcc把这个函数扔到init section
void my_fini(void) __attribute__((destructor));  //告诉gcc把这个函数扔到fini section
#endif

CSceneCore::CSceneCore():
    m_pSceneGraphManger(new CSceneGraphManager)
{
    InitSystem();
}

CSceneCore::~CSceneCore()
{
    delete m_pSceneGraphManger;
}

ISceneGraphManager *CSceneCore::GetSceneGraphManager()
{
    return(m_pSceneGraphManger);
}

/// 初始化系统
void CSceneCore::InitSystem()
{
    InitLog();
}

void CSceneCore::InitLog()
{
    /// 将警告信息输出到文件中
    osg::NotifyHandler* pHandler = new CMyNotify;
    osg::setNotifyHandler(pHandler);
    osgEarth::setNotifyHandler(pHandler);
}

/// 获取场景核心
ISceneCore *GetSceneCore()
{
    if(!s_gBChecked)
    {
        return(nullptr);
    }
    else
    {
        if(nullptr == s_gMapCore)
        {
            s_gMapCore = new CSceneCore;
        }
        return(s_gMapCore);
    }
}

/// 检查硬件是否满足要求
bool CheckPC()
{
    if(!s_gBChecked)
    {
        osgEarth::setNotifyLevel(osg::INFO);
        s_gBChecked = osgEarth::Registry::instance()->getCapabilities().supportsGLSL();
        osgEarth::Registry::instance()->unRefImageDataAfterApply() = false;
        osg::setNotifyLevel(osg::WARN);
        osgEarth::setNotifyLevel(osg::NOTICE);
    }

    return(s_gBChecked);
}

/// 配置路径
void SetExePath(const string& sPath)
{
    s_strPath = sPath;
#ifdef Q_OS_WIN
	s_strPath += '\\';
#else
    s_strPath += '/';
#endif
}

/// 获取路径
const string& GetExePath()
{
    return(s_strPath);
}
