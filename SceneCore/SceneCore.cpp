#include <QtCore/qsystemdetection.h>
#include <QSurfaceFormat>
#include <osg/DeleteHandler>
#include <osgEarth/Registry>
#include <osgEarth/Capabilities>
#include <osgDB/Registry>
#include "MyNotify.h"
#include "SceneCore.h"
#include "SceneGraph/SceneGraphManager.h"

#ifdef Q_OS_WIN
static const char s_cSeparator('\\');
#else
static const char s_cSeparator('/');
#endif
static CSceneCore* s_gMapCore=nullptr;
static bool        s_gBChecked(false);
static std::string s_strPath("./");
static std::string s_strData("./Data/");

CSceneCore::CSceneCore():
    m_pSceneGraphManger(new CSceneGraphManager)
{
}

CSceneCore::~CSceneCore()
{
    delete m_pSceneGraphManger;
}

ISceneGraphManager *CSceneCore::GetSceneGraphManager()
{
    return(m_pSceneGraphManger);
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

/// 移除核心
void CloseSceneCore()
{
    if(nullptr != s_gMapCore)
    {
        delete s_gMapCore;
        s_gMapCore = nullptr;
    }
}

/// 检查硬件是否满足要求
bool CheckPC(char *argv[])
{
    if(!s_gBChecked)
    {
        const char *pStr = strrchr(argv[0],s_cSeparator);

        if(nullptr == pStr)
        {
            return(false);
        }

        s_strPath = std::string(argv[0],pStr-argv[0]+1);

        /// 获取数据路径
        s_strData = s_strPath;
        s_strData += "Data";
        s_strData += s_cSeparator;

        /// 将警告信息输出到文件中
        osg::NotifyHandler* pHandler = new CMyNotify(s_strData+"Log"+s_cSeparator,pStr);
        osg::setNotifyHandler(pHandler);
        osgEarth::setNotifyHandler(pHandler);

        osgEarth::setNotifyLevel(osg::INFO);
        const osgEarth::Capabilities& csCapabilities = osgEarth::Registry::instance()->getCapabilities();
        s_gBChecked = csCapabilities.supportsGLSL();
        if(!s_gBChecked)
        {
            return(s_gBChecked);
        }

        osg::setNotifyLevel(osg::WARN);
        osgEarth::setNotifyLevel(osg::NOTICE);

        int nMax,nMin;
        sscanf(csCapabilities.getVersion().data(),"%d.%d",&nMax,&nMin);

        QSurfaceFormat format = QSurfaceFormat::defaultFormat();
        osg::GraphicsContext::Traits traits(osg::DisplaySettings::instance().get());

        /// 由于FBX采用的读取方法是单线程的，开启多线程会崩溃
        osg::DisplaySettings::instance()->setNumOfDatabaseThreadsHint(OpenThreads::GetNumberOfProcessors()-3);
        int nHttpThreads = osg::DisplaySettings::instance()->getNumOfDatabaseThreadsHint() - 4;
        if(nHttpThreads < 1)
        {
            nHttpThreads = 1;
        }
        osg::DisplaySettings::instance()->setNumOfHttpDatabaseThreadsHint(nHttpThreads);
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
        osgDB::Registry::instance()->getObjectWrapperManager()->findWrapper("osg::Image");
    }

    return(s_gBChecked);
}

#include <QTextCodec>
QTextCodec* g_utf8 = QTextCodec::codecForName("UTF-8");
QTextCodec* g_gb2312 = QTextCodec::codecForName("GB2312");
/// 转成utf-8
std::string Convert2Utf8(const std::string& sPath)
{
    QTextCodec::ConverterState tmpState;
    QString sText = g_utf8->toUnicode(sPath.data(),sPath.size(),&tmpState);
    if(tmpState.invalidChars > 0)
    {
        tmpState.invalidChars = 0;
        sText = g_gb2312->toUnicode(sPath.data(),sPath.size(),&tmpState);
        if(tmpState.invalidChars > 0)
        {
            auto codeList = QTextCodec::availableMibs();
            for(int i=codeList.size()-1;tmpState.invalidChars>0 && i>-1;--i)
            {
                tmpState.invalidChars = 0;
                sText = QTextCodec::codecForMib(codeList[i])->toUnicode(sPath.data(),sPath.size(),&tmpState);
            }
        }
    }
    return sText.toUtf8().toStdString();
}

/// 转成local
std::string Convert2Local(const std::string& sPath)
{
    QTextCodec::ConverterState tmpState;
    QString sText = g_utf8->toUnicode(sPath.data(),sPath.size(),&tmpState);
    if(tmpState.invalidChars > 0)
    {
        tmpState.invalidChars = 0;
        sText = g_gb2312->toUnicode(sPath.data(),sPath.size(),&tmpState);
        if(tmpState.invalidChars > 0)
        {
            auto codeList = QTextCodec::availableMibs();
            for(int i=codeList.size()-1;tmpState.invalidChars>0 && i>-1;--i)
            {
                tmpState.invalidChars = 0;
                sText = QTextCodec::codecForMib(codeList[i])->toUnicode(sPath.data(),sPath.size(),&tmpState);
            }
        }
    }
    return sText.toLocal8Bit().toStdString();
}

#ifdef NEED_VR
#include <openvr/openvr.h>
bool CheckVR()
{
    return(vr::VR_IsRuntimeInstalled() && vr::VR_IsHmdPresent());
}
#endif

/// 获取路径
const std::string& GetExePath()
{
    return(s_strPath);
}

const std::string& GetDataPath()
{
    return(s_strData);
}
