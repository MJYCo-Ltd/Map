#include <osgEarth/CullingUtils>
#include "SceneLine.h"

void CSceneLine::InitNode()
{
    osg::Group *pGroup = new osg::Group;
    m_pLine = new osgEarth::LineDrawable;
    m_pLine->setDataVariance(osg::Object::DYNAMIC);
    pGroup->addChild(m_pLine);
    pGroup->addCullCallback(new osgEarth::InstallCameraUniform);
    m_pLine->setColor(osg::Vec4(m_stColor.fR,m_stColor.fG,m_stColor.fB,m_stColor.fA));
    m_pLine->setLineWidth(m_nLineWidth);
    m_pLine->setStipplePattern(0xffffu);
    m_pLine->setLineSmooth(true);

    ImplSceneNode<ILine>::InitNode();
    ImplSceneNode<ILine>::SetOsgNode(pGroup);
}

void CSceneLine::UpdateNode()
{
    if(m_bWidthChanged)
    {
        m_pLine->setLineWidth(m_nLineWidth);
        m_bWidthChanged=false;
    }

    if(m_bColorChanged)
    {
        m_pLine->setColor(osg::Vec4(m_stColor.fR,m_stColor.fG,m_stColor.fB,m_stColor.fA));
        m_bColorChanged = false;
    }

    if(m_bShapeChanged)
    {
        m_pLine->clear();

        std::vector<ScenePos> vAllConverdPos;
        if(nullptr != m_pDealPoint && m_pDealPoint->Conversion(m_listAllPos,vAllConverdPos))
        {
            for(auto one : vAllConverdPos)
            {
                m_pLine->pushVertex(osg::Vec3(one.fX,one.fY,one.fZ));
            }
        }
        else
        {
            for(auto one : m_listAllPos)
            {
                m_pLine->pushVertex(osg::Vec3(one.fX,one.fY,one.fZ));
            }
        }
        m_pLine->setFirst(0);
        m_pLine->finish();
        m_bShapeChanged=false;
    }

    if(m_bLineTypeChanged)
    {
        m_pLine->setStipplePattern(DOTTED_LINE == m_emLineType ? 0xF0F0 : 0xFFFF);
        m_bLineTypeChanged=false;
    }

    ImplSceneNode<ILine>::UpdateNode();
}
