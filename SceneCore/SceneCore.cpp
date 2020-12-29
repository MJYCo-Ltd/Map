#include <QtCore>
#include <QSurfaceFormat>
#include <osgEarth/Registry>
#include <osgEarth/Capabilities>
#include <osgDB/ReadFile>
#include "MyNotify.h"
#include "SceneCore.h"
#include "SceneGraph/SceneGraphManager.h"

static CSceneCore* s_gMapCore=nullptr;
static bool        s_gBChecked(false);
static std::string s_strPath(".");

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
        const osgEarth::Capabilities& csCapabilities = osgEarth::Registry::instance()->getCapabilities();
        s_gBChecked = csCapabilities.supportsGLSL();

        osgEarth::Registry::instance()->unRefImageDataAfterApply() = false;
        osg::setNotifyLevel(osg::WARN);
        osgEarth::setNotifyLevel(osg::NOTICE);

        int nMax,nMin;
        sscanf_s(csCapabilities.getVersion().data(),"%d.%d",&nMax,&nMin);

        QSurfaceFormat format = QSurfaceFormat::defaultFormat();
        osg::GraphicsContext::Traits traits(osg::DisplaySettings::instance().get());
        format.setAlphaBufferSize( traits.alpha );
        format.setRedBufferSize( traits.red );
        format.setGreenBufferSize( traits.green );
        format.setBlueBufferSize( traits.blue );
        format.setDepthBufferSize( traits.depth );
        format.setStencilBufferSize( traits.stencil );
        format.setSamples( traits.samples );
        format.setVersion(nMax,nMin);

        /// 判断是否支持核心模式
        if(csCapabilities.isCoreProfile())
        {
            format.setProfile(QSurfaceFormat::CoreProfile);
        }
        else
        {
            format.setProfile(QSurfaceFormat::CompatibilityProfile);
        }

        format.setSwapInterval( traits.vsync ? 1 : 0 );
        format.setStereo( traits.quadBufferStereo ? 1 : 0 );

        QSurfaceFormat::setDefaultFormat(format);
    }

    return(s_gBChecked);
}

/// 配置路径
void SetExePath(const std::string& sPath)
{
    s_strPath = sPath;
#ifdef Q_OS_WIN
	s_strPath += '\\';
#else
    s_strPath += '/';
#endif
}

/// 获取路径
const std::string& GetExePath()
{
    return(s_strPath);
}
