#include <Inner/OsgExtern/OsgExtern.h>
#include <osgEarth/ScreenSpaceLayout>
#include "SceneLabel.h"

/// 设置绑定的节点
bool CSceneLabel::SetAttachNode(ISceneNode *pSceneNode)
{
    if(nullptr !=pSceneNode && m_pSceneGraph == pSceneNode->GetBoundSceneGraph())
    {
        if(nullptr != m_pAttachNode)
        {
            m_pAttachNode->AsOsgSceneNode()->RemoveAttach(this);
        }
        m_pAttachNode = pSceneNode;
        m_pAttachNode->AsOsgSceneNode()->BindAttach(this);

        m_bPosChanged = true;
        auto bound = m_pAttachNode->AsOsgSceneNode()->GetRealNode()->getBound();
        osg::Vec3 pos = bound._center;
        pos.x() += bound._radius;
        m_stPos.dX = pos.x();
        m_stPos.dY = pos.y();
        m_stPos.dZ = pos.z();

        ImplSceneNode<ILabel>::NodeChanged();

        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(
                    new COSGAttachNode(
                        pSceneNode->AsOsgSceneNode()->GetRealNode()
                        ,GetRealNode()
                        ,true));
        return(true);
    }

    return(false);
}

/// 取消绑定
bool CSceneLabel::DisAttachNode(ISceneNode *pSceneNode)
{
    if(nullptr != pSceneNode && m_pAttachNode == pSceneNode)
    {
        pSceneNode->AsOsgSceneNode()->RemoveAttach(this);

        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(
                    new COSGAttachNode(
                        pSceneNode->AsOsgSceneNode()->GetRealNode()
                        ,GetRealNode()
                        ,false));
        return(true);
    }

    return(false);
}

void CSceneLabel::NodeVisibleChanged(bool bVisible)
{
    ImplSceneNode<ILabel>::SetVisible(bVisible);
}

void CSceneLabel::AddIntoParent(osg::Group *pParent)
{
    ImplSceneNode<ILabel>::AddNode(pParent,GetRealNode());
}

void CSceneLabel::DelFromParent(osg::Group *pParent)
{
    ImplSceneNode<ILabel>::DelNode(pParent,GetRealNode());
}

/// 初始化节点
void CSceneLabel::InitNode()
{
    ImplSceneNode<ILabel>::InitNode();
    m_bOpenCull=false;
    m_pProgramNode->setCullingActive(false);

    m_pText = new osgEarth::Text;
    m_pText->setDataVariance(osg::Object::DYNAMIC);

    /// 设置初始颜色
    m_stOutColor.fR=m_stOutColor.fG=m_stOutColor.fB=0.f;
    m_stBoundingBoxColor.fR=m_stBoundingBoxColor.fB=0.f;

    /// 设置字体大小
    m_pText->setCharacterSize(m_fFontSize);
    m_pText->setAlignment(osgText::Text::LEFT_CENTER);
    m_pText->setAxisAlignment(osgText::Text::SCREEN);
    m_pText->setCharacterSizeMode(osgText::Text::SCREEN_COORDS);
    m_pText->setDrawMode(osgText::Text::TEXT);
    m_pText->setBoundingBoxColor(osg::Vec4(m_stBoundingBoxColor.fR,m_stBoundingBoxColor.fG
                                           ,m_stBoundingBoxColor.fB,m_stBoundingBoxColor.fA));
    m_pText->setBackdropType(osgText::Text::OUTLINE);
    ImplSceneNode<ILabel>::SetOsgNode(m_pText.get());
}

/// 更新节点
void CSceneLabel::FrameCall()
{
    if(m_bFontChanged)
    {
        auto pFont = m_pSceneGraph->ResouceLoader()->LoadFont(m_sFont);
        if(nullptr != pFont)
        {
            m_pText->setFont(pFont);
        }
        m_bFontChanged = false;
    }

    if(m_bInfoChanged)
    {
        m_pText->setText(m_sInfo,osgText::String::ENCODING_UTF8);
        m_bInfoChanged=false;
    }

    if(m_bFontSizeChanged)
    {
        m_pText->setCharacterSize(m_fFontSize);
        m_bFontSizeChanged=false;
    }

    if(m_bFillColorChanged)
    {
        m_pText->setColor(osg::Vec4(m_stFillColor.fR,m_stFillColor.fG
                                    ,m_stFillColor.fB,m_stFillColor.fA));
        m_bFillColorChanged=false;
    }

    if(m_bOutColorChanged)
    {
        m_pText->setBackdropColor(osg::Vec4(m_stFillColor.fR,m_stFillColor.fG
                                    ,m_stFillColor.fB,m_stFillColor.fA));
        m_bOutColorChanged=false;
    }

    if(m_bPosChanged)
    {
        m_pText->setPosition(osg::Vec3(m_stPos.dX,m_stPos.dY,m_stPos.dZ));
        m_bPosChanged = false;
    }

    if(m_bBoundingBoxVisibleChanged)
    {
        if(m_bShowBoundingBox)
        {
            m_pText->setDrawMode(osgText::Text::TEXT|osgText::Text::BOUNDINGBOX
                                 |osgText::Text::FILLEDBOUNDINGBOX);
        }
        else
        {
            m_pText->setDrawMode(osgText::Text::TEXT);
        }
        m_bBoundingBoxVisibleChanged=false;
    }

    if(m_bBoudingBoxColorChanged)
    {
        m_pText->setBoundingBoxColor(osg::Vec4(m_stBoundingBoxColor.fR,m_stBoundingBoxColor.fG
                                               ,m_stBoundingBoxColor.fB,m_stBoundingBoxColor.fA));
        m_bBoudingBoxColorChanged=false;
    }

    if(m_bPixelOffChanged)
    {
        auto pLayoutData=osgEarth::ScreenSpaceLayoutData::getOrCreate(m_pText.get());
        pLayoutData->setPixelOffset(osg::Vec2s(m_stPixelOffset.sWidth,m_stPixelOffset.sHeight));
        m_bPixelOffChanged=false;
    }

    ImplSceneNode<ILabel>::FrameCall();
}
