#include "SceneImage.h"
#include <osgDB/WriteFile>
std::map<ISceneGraph*,std::map<std::string,osg::observer_ptr<osg::Geometry>>> CSceneImage::s_mapID2ImageNode;

CSceneImage::~CSceneImage()
{
    /// 移除没有用的节点
    for(auto one=s_mapID2ImageNode[m_pSceneGraph].begin();one!=s_mapID2ImageNode[m_pSceneGraph].end();)
    {
        if(!one->second.valid())
        {
            one = s_mapID2ImageNode[m_pSceneGraph].erase(one);
        }
        else
        {
            ++one;
        }
    }
}

/// 设置QImage
void CSceneImage::SetQImage(const QImage &rImage)
{
    m_pQImageDrawNode = TryChangeToGeomerty(m_pSceneGraph->ResouceLoader()->CreateImageNode(rImage));
    SET_TRUE_NODE_UPDATE(m_bGeomertyChanged)
}

/// 初始化节点
void CSceneImage::InitNode()
{
    ImplSceneNode<IImage>::InitNode();
    m_bOpenCull=false;
    m_pProgramNode->setCullingActive(m_bOpenCull);
    m_pProgramNode->setStateSet(m_pSceneGraph->ResouceLoader()->GetOrCreateStateSet("GLSL/Global.glsl"));
}

void CSceneImage::FrameCall()
{
    osg::ref_ptr<osg::Geometry> pDrawGeomerty{};
    if(m_bGeomertyChanged)
    {
        pDrawGeomerty = m_pQImageDrawNode;
        m_pQImageDrawNode=nullptr;
        m_bGeomertyChanged=false;
    }

    if(m_bPathChanged)
    {
        pDrawGeomerty = TryChangeToGeomerty(m_pSceneGraph->ResouceLoader()->CreateImageNode(m_sImagePath));
        m_bPathChanged=false;
    }

    if(m_bImageDataChanged)
    {
        pDrawGeomerty = TryChangeToGeomerty(m_pSceneGraph->ResouceLoader()->CreateImageNode(&(IImage::m_stRGBAData)));
        m_bImageDataChanged=false;
    }

    /// 如果图片没有更新则读取原来的图片
    if(!pDrawGeomerty.valid())
    {
        pDrawGeomerty = TryChangeToGeomerty(m_pRootNode);
    }

    /// 如果节点有效
    if(pDrawGeomerty.valid())
    {
        void* ss = reinterpret_cast<void*>(pDrawGeomerty.get());
        std::stringstream ss1;
        ss1 << ss ;
        ss1 << '_';

        ss1<<"Red_"<<int(m_stColor.fR*255)
           <<"_Green_"<<int(m_stColor.fG*255)
           <<"_Blue_"<<int(m_stColor.fB*255)
           <<"_Alpha_"<<int(m_stColor.fA*255);
        ss1<<'_';
        ss1<<"width_"<<m_stImageSize.unWidth<<"height_"<<m_stImageSize.unHeight;

        auto pFindOne = s_mapID2ImageNode[m_pSceneGraph].find(ss1.str());
        if(s_mapID2ImageNode[m_pSceneGraph].end() != pFindOne && pFindOne->second.valid())
        {
            SetOsgNode(pFindOne->second.get());
        }
        else
        {
            osg::ref_ptr<osg::Geometry> pNewNode;
            if(s_mapID2ImageNode[m_pSceneGraph].empty())
            {
                pNewNode = pDrawGeomerty;
            }
            else
            {
                pNewNode = reinterpret_cast<osg::Geometry*>(pDrawGeomerty->clone(osg::CopyOp::DEEP_COPY_ARRAYS
                                                                                      |osg::CopyOp::DEEP_COPY_PRIMITIVES
                                                                                      |osg::CopyOp::DEEP_COPY_STATESETS));
            }

            static_cast<osg::Vec4Array*>(pNewNode->getColorArray())
                    ->at(0).set(m_stColor.fR,m_stColor.fG,m_stColor.fB,m_stColor.fA);

            if(m_stImageSize.bOutSet)
            {
                auto pVertexArray = reinterpret_cast<osg::Vec3Array*>(pNewNode->getVertexArray());
                int nX(m_stImageSize.unWidth/2);
                int nY(m_stImageSize.unHeight/2);

                pVertexArray->at(0).set(-nX,-nY,0);
                pVertexArray->at(1).set(-nX,nY,0);
                pVertexArray->at(2).set(nX,-nY,0);
                pVertexArray->at(3).set(nX,nY,0);
            }

            SetOsgNode(pNewNode);
            s_mapID2ImageNode[m_pSceneGraph][ss1.str()] = pNewNode;
        }
    }

    ImplSceneNode<IImage>::FrameCall();
}

/// 试着变成绘制体
osg::Geometry *CSceneImage::TryChangeToGeomerty(osg::Node* pNode)
{
    if(nullptr != pNode)
    {
        return(pNode->asGeometry());
    }

    return(nullptr);
}
