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
    m_pTerrain = s_pMapNode->getTerrain();
    m_pMapSrs = s_pMapNode->getMapSRS();

    m_pHorizonCullBack->setEnabled(s_pMapNode->isGeocentric());

    /// 转换到
    if (!m_pMapSrs->isEquivalentTo(m_pGeoPoint.getSRS()))
    {
        osgEarth::GeoPoint p = m_pGeoPoint.transform(m_pMapSrs.get());

        /// 如果有高程
        if(m_pTerrain.valid())
        {
//            p.makeRelative(m_pTerrain.get());
        }

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

/// 更新
void CMapLocation::UpdateNode()
{
    if (m_pMapSrs.valid() && !m_pMapSrs->isEquivalentTo(m_pGeoPoint.getSRS()))
    {
        osgEarth::GeoPoint p = m_pGeoPoint.transform(m_pMapSrs.get());

        /// 如果有高程
        if(m_pTerrain.valid())
        {
//            p.makeRelative(m_pTerrain.get());
        }

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

    ImplSceneNode<IMapLocation>::UpdateNode();
}

/// 初始化节点
void CMapLocation::InitNode()
{
    ImplSceneNode<IMapLocation>::InitNode();
    m_pGeoTransform = new CMyMatrixTransform;

    m_pHorizonCullBack = new osgEarth::HorizonCullCallback;
    m_pGeoTransform->addCullCallback(m_pHorizonCullBack);

    SetOsgNode(m_pGeoTransform.get());
}

/// 位置更改
void CMapLocation::PosChanged()
{
    if(!m_pGeoPoint.isValid())
    {
        m_pGeoPoint.set(osgEarth::SpatialReference::get("wgs84"),m_stGeoPos.fX,m_stGeoPos.fY,m_stGeoPos.fZ,osgEarth::ALTMODE_ABSOLUTE);
    }
    else
    {
        m_pGeoPoint.x() = m_stGeoPos.fX;
        m_pGeoPoint.y() = m_stGeoPos.fY;
        m_pGeoPoint.z() = m_stGeoPos.fZ;
    }

    NodeChanged();
}

void CMapLocation::TerrainTypeChanged()
{
}
