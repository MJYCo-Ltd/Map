#include <Inner/OsgExtern/OsgExtern.h>
#include "MapLocation.h"

/// 更新地图节点
void CMapLocation::SetSceneNode(ISceneNode *pNode)
{
    if(nullptr != m_pSceneNode)
    {
        DelNode(m_pGeoTransform.get(),m_pSceneNode->AsOsgSceneNode()->GetOsgNode());
    }

    /// 如果两者不是同一个渲染器,则返回
    if (pNode->GetBoundSceneGraph() != m_pSceneGraph)
    {
        return;
    }

    m_pSceneNode = pNode;

    AddNode(m_pGeoTransform.get(),pNode->AsOsgSceneNode()->GetOsgNode());
}

void CMapLocation::UpdateMapNode()
{
    ImplMapSceneNode<IMapLocation>::UpdateMapNode();

    /// 转换到
    ReCalculatMatrix();
}

/// 更新
void CMapLocation::UpdateNode()
{
    if(m_bPosChanged)
    {
        ReCalculatMatrix();
        m_bPosChanged = false;
    }

    ImplSceneNode<IMapLocation>::UpdateNode();
}

/// 初始化节点
void CMapLocation::InitNode()
{
    ImplMapSceneNode<IMapLocation>::InitNode();
    m_pGeoTransform = new CMyMatrixTransform;
    ImplMapSceneNode<IMapLocation>::SetMapSceneNode(m_pGeoTransform.get());
}

/// 位置更改
void CMapLocation::PosChanged()
{
    if(!m_pGeoPoint.isValid())
    {
        m_pGeoPoint.set(IOsgMapSceneNode::s_pWGS84,m_stGeoPos.fX,m_stGeoPos.fY,m_stGeoPos.fZ,osgEarth::ALTMODE_ABSOLUTE);
    }
    else
    {
        m_pGeoPoint.x() = m_stGeoPos.fX;
        m_pGeoPoint.y() = m_stGeoPos.fY;
        m_pGeoPoint.z() = m_stGeoPos.fZ;
    }

    m_bPosChanged = true;
    NodeChanged();
}

void CMapLocation::TerrainTypeChanged()
{
}

///重新计算矩阵
void CMapLocation::ReCalculatMatrix()
{
    if (!IOsgMapSceneNode::s_pMapSRS->isEquivalentTo(m_pGeoPoint.getSRS()))
    {
        osgEarth::GeoPoint p = m_pGeoPoint.transform(IOsgMapSceneNode::s_pMapSRS.get());

        osg::Matrixd local2world;
        p.createLocalToWorld( local2world );
        m_pGeoTransform->setMatrix(local2world);
    }
    else
    {
        osg::Matrixd local2world;
        m_pGeoPoint.createLocalToWorld(local2world );
        m_pGeoTransform->setMatrix(local2world);
    }
}
