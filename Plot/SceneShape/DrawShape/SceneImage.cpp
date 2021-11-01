#include "SceneImage.h"
#include <osgDB/WriteFile>
std::map<std::string,osg::observer_ptr<osg::Geometry>> CSceneImage::s_mapID2ImageNode;

CSceneImage::~CSceneImage()
{
    /// 移除没有用的节点
    for(auto one=s_mapID2ImageNode.begin();one!=s_mapID2ImageNode.end();)
    {
        if(!one->second.valid())
        {
            one = s_mapID2ImageNode.erase(one);
        }
        else
        {
            ++one;
        }
    }
}

void CSceneImage::ImageSizeChanged()
{
    if(m_stImageSize)
    {
        m_stImageSize.bOutSet = true;
        SET_TRUE_NODE_UPDATE(m_bSizeChanged)
    }
}

void CSceneImage::SetQImage(const QImage &rImage)
{
    m_pQImageDrawNode = m_pSceneGraph->ResouceLoader()->CreateImageNode(rImage)->asGeometry();
    SET_TRUE_NODE_UPDATE(m_bGeomertyChanged)
}

void CSceneImage::InitNode()
{
    ImplSceneNode<IImage>::InitNode();
    m_bOpenCull=false;
    m_pProgramNode->setCullingActive(m_bOpenCull);
    m_pProgramNode->setStateSet(m_pSceneGraph->ResouceLoader()->GetOrCreateStateSet("GLSL/Global.glsl"));
}

void CSceneImage::FrameCall()
{
    if(m_bGeomertyChanged)
    {
        m_pDrawNode = m_pQImageDrawNode;
        SetOsgNode(m_pDrawNode.get());
        m_pQImageDrawNode=nullptr;
        m_bGeomertyChanged = false;
    }

    if(m_bPathChanged)
    {
        m_pDrawNode = m_pSceneGraph->ResouceLoader()->CreateImageNode(m_sImagePath)->asGeometry();
        SetOsgNode(m_pDrawNode.get());
        m_bPathChanged =false;
    }

    if(m_bImageDataChanged)
    {
        m_pDrawNode = m_pSceneGraph->ResouceLoader()->CreateImageNode(&(IImage::m_stRGBAData))->asGeometry();
        SetOsgNode(m_pDrawNode.get());
        m_bImageDataChanged = false;
    }

    if(m_pDrawNode.valid())
    {
        void* ss = reinterpret_cast<void*>(m_pDrawNode.get());
        std::stringstream ss1;
        ss1 << ss ;
        ss1 << '_';

        if(m_bColorChanged)
        {
            ss1<<(int)(m_stColor.fR*1e3+m_stColor.fG*1e2+m_stColor.fB*10+m_stColor.fA);
            ss1<<'_';
        }

        if(m_bSizeChanged)
        {
            ss1<<"width_"<<m_stImageSize.unWidth<<"height_"<<m_stImageSize.unHeight;
        }

        /// 如果颜色修改或者大小修改
        if(m_bColorChanged || m_bSizeChanged)
        {
            auto pFindOne = s_mapID2ImageNode.find(ss1.str());
            if(s_mapID2ImageNode.end() != pFindOne && pFindOne->second.valid())
            {
                SetOsgNode(pFindOne->second.get());
            }
            else
            {
                auto pNewNode = reinterpret_cast<osg::Geometry*>(m_pDrawNode->clone(osg::CopyOp::DEEP_COPY_ARRAYS
                                                                           |osg::CopyOp::DEEP_COPY_PRIMITIVES));

                if(m_bColorChanged)
                {
                    static_cast<osg::Vec4Array*>(pNewNode->getColorArray())
                            ->at(0).set(m_stColor.fR,m_stColor.fG,m_stColor.fB,m_stColor.fA);
                    m_bColorChanged = false;
                }

                if(m_bSizeChanged)
                {
                    auto pVertexArray = reinterpret_cast<osg::Vec3Array*>(pNewNode->getVertexArray());
                    int nX(m_stImageSize.unWidth/2);
                    int nY(m_stImageSize.unHeight/2);

                    pVertexArray->at(0).set(-nX,-nY,0);
                    pVertexArray->at(1).set(-nX,nY,0);
                    pVertexArray->at(2).set(nX,-nY,0);
                    pVertexArray->at(3).set(nX,nY,0);
                    m_bSizeChanged=false;
                }

                SetOsgNode(pNewNode);
                s_mapID2ImageNode[ss1.str()] = pNewNode;
            }
        }
    }

    ImplSceneNode<IImage>::FrameCall();
}
