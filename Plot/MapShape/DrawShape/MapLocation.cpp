#include <Inner/OsgExtern/OsgExtern.h>
#include "MapLocation.h"

/// 更新地图节点
void CMapLocation::SetSceneNode(ISceneNode *pNode)
{
    if(nullptr != m_pSceneNode)
    {
        auto update = new CModifyNode(m_pGeoTransform.get(), m_pSceneNode->AsOsgSceneNode()->GetOsgNode(),false);
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(update);
    }

    /// 如果两者不是同一个渲染器,则返回
    if (pNode->GetBoundSceneGraph() != m_pSceneGraph)
    {
        return;
    }

    m_pSceneNode = pNode;

    auto update = new CModifyNode(m_pGeoTransform.get(), pNode->AsOsgSceneNode()->GetOsgNode(),true);
    m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(update);
}

void CMapLocation::UpdateMapNode(osgEarth::MapNode *pMapNode)
{
    m_pTerrain = pMapNode->getTerrain();
    m_pMapSrs = pMapNode->getMapSRS();

    m_pHorizonCullBack->setEnabled(pMapNode->isGeocentric());

    /// 转换到
    if (!m_pMapSrs->isEquivalentTo(m_pGeoPoint.getSRS()))
    {
        osgEarth::GeoPoint p = m_pGeoPoint.transform(m_pMapSrs.get());

        /// 如果有高程
        if(m_pTerrain.valid())
        {
            p.makeAbsolute(m_pTerrain.get());
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

/// 初始化节点
void CMapLocation::InitNode()
{
    ImplMapSceneNode<IMapLocation>::InitNode();
    m_pGeoTransform = new CMyMatrixTransform;

    auto pSate = m_pGeoTransform->getOrCreateStateSet();
    m_pVirutlProgram = osgEarth::VirtualProgram::getOrCreate(pSate);
    m_pSceneGraph->ResouceLoader()->LoadVirtualProgram(m_pVirutlProgram,"Data/GLSL/Global.glsl");
    m_pHorizonCullBack = new osgEarth::HorizonCullCallback;
    m_pGeoTransform->addCullCallback(m_pHorizonCullBack);

    SetOsgNode(m_pGeoTransform.get());
}

/// 位置更改
void CMapLocation::PosChanged()
{
    if(!m_pGeoPoint.isValid())
    {
        m_pGeoPoint.set(osgEarth::SpatialReference::get("wgs84"),m_stGeoPos.fLon,m_stGeoPos.fLat,m_stGeoPos.fHeight,osgEarth::ALTMODE_RELATIVE);
    }
    else
    {
        m_pGeoPoint.x() = m_stGeoPos.fLon;
        m_pGeoPoint.y() = m_stGeoPos.fLat;
        m_pGeoPoint.z() = m_stGeoPos.fHeight;
    }

    if (m_pMapSrs.valid() && !m_pMapSrs->isEquivalentTo(m_pGeoPoint.getSRS()))
    {
        osgEarth::GeoPoint p = m_pGeoPoint.transform(m_pMapSrs.get());

        /// 如果有高程
        if(m_pTerrain.valid())
        {
            p.makeAbsolute(m_pTerrain.get());
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
