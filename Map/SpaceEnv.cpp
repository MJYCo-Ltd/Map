#include <QLibrary>
#include <SpaceEnv/ISpaceBackGround.h>
#include "SpaceEnv.h"

CSpaceEnv::CSpaceEnv(ISceneGraph *pSceneGraph):
    QtOsgSceneNode<ISpaceEnv>(pSceneGraph)
{
}

/// 设置主相机
void CSpaceEnv::SetMainCamara(osg::Camera *pCamera)
{
    m_pMainCamera = pCamera;
    m_pMainCamera->setClearMask(GL_DEPTH_BUFFER_BIT);
}

ISpaceBackGround *CSpaceEnv::GetSpaceBackGround()
{
    return(m_pSpaceBackGround);
}

/// 控制空间背景的显隐
void CSpaceEnv::ShowSpaceBackGround(bool bShow)
{
    if(bShow != m_bShowSpaceBackGround)
    {
        m_bShowSpaceBackGround = bShow;

        if(m_bShowSpaceBackGround)
        {
            AddSceneNode(m_pSpaceBackGround);
            m_pMainCamera->setClearMask(GL_DEPTH_BUFFER_BIT);
        }
        else
        {
            RemoveSceneNode(m_pSpaceBackGround);
            m_pMainCamera->setClearMask(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
        }
    }
}

/// 初始化节点
void CSpaceEnv::InitSceneNode()
{
    QtOsgSceneNode<ISpaceEnv>::InitSceneNode();

    LoadBackGround();
}

void CSpaceEnv::LoadBackGround()
{
    if(nullptr == m_pSpaceBackGround)
    {
        typedef ISpaceBackGround* (*CreateSpaceEnvFun)(ISceneGraph*);
#ifdef Q_OS_WIN

#ifdef QT_NO_DEBUG
        QLibrary loadMap("SpaceBackGround.dll");
#else
        QLibrary loadMap("SpaceBackGroundd.dll");
#endif

#else

#ifdef QT_NO_DEBUG
        QLibrary loadMap("libSpaceBackGround.so");
#else
        QLibrary loadMap("libSpaceBackGroundd.so");
#endif

#endif
        if(loadMap.load())
        {
            CreateSpaceEnvFun pCreateSpaceEnv = reinterpret_cast<CreateSpaceEnvFun>(loadMap.resolve("CreateSpaceEnv"));
            if(nullptr != pCreateSpaceEnv)
            {
                m_pSpaceBackGround = pCreateSpaceEnv(m_pSceneGraph);
                dynamic_cast<IOsgSceneNode*>(m_pSpaceBackGround)->InitSceneNode();
                AddSceneNode(m_pSpaceBackGround);
            }
        }
    }
}
