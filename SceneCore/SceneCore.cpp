#include <osgEarth/Registry>
#include <osgEarth/Capabilities>
#include <osgDB/ReadFile>
#include "MyNotify.h"
#include "SceneCore.h"
#include "SceneGraph/SceneGraphManager.h"
#include "Message/MessageManager.h"
static CSceneCore s_gMapCore;
static bool        s_gBChecked(false);
static string     s_strPath;

CSceneCore::CSceneCore():
    m_pSceneGraphManger(new CSceneGraphManager),
    m_pMessageManager(new CMessageManager)
{
    InitSystem();
}

CSceneCore::~CSceneCore()
{
    delete m_pSceneGraphManger;
    delete m_pMessageManager;
}

ISceneGraphManager *CSceneCore::GetSceneGraphManager()
{
    return(m_pSceneGraphManger);
}

IMessageManager *CSceneCore::GetMessageManager()
{
    return(m_pMessageManager);
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
        return(&s_gMapCore);
    }
}

/// 检查硬件是否满足要求
bool CheckPC()
{
    if(!s_gBChecked)
    {
        osgEarth::setNotifyLevel(osg::INFO);
        s_gBChecked = osgEarth::Registry::instance()->getCapabilities().supportsGLSL();
        osgEarth::setNotifyLevel(osg::NOTICE);
    }

    return(s_gBChecked);
}

/// 配置路径
void SetExePath(const string sPath)
{
    s_strPath = sPath;
	s_strPath += '\\';
}

/// 获取路径
const string& GetExePath()
{
    return(s_strPath);
}
