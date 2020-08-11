#include "SceneModel.h"

CSceneModel::CSceneModel(ISceneGraph *pSceneGraph):
    QtDrawShape<IModel>(pSceneGraph)
{
}

/// 更新地图节点
void CSceneModel::UpdateMapNode(osgEarth::MapNode *pMapNode)
{
    if(pMapNode->isGeocentric())
    {
        if(!m_pModel.valid())
        {
            m_pModel = new osgEarth::ModelNode(pMapNode,m_styleNode);
            ///gengxinweizhi
            PosChanged();

            m_pLodNode->addChild(m_pModel,0.f,FLT_MAX);
        }
        else
        {
            m_pModel->setHorizonCulling(true);
        }
    }
    else
    {
    }
}

/// 初始化节点
void CSceneModel::InitSceneNode()
{
    QtDrawShape<IModel>::InitSceneNode();

    m_pLodNode = new osg::LOD;
    m_pOsgNode->addChild(m_pLodNode);
}

void CSceneModel::PosChanged()
{
    ///
    if(m_pModel.valid())
    {
        m_pModel->setPosition(osgEarth::GeoPoint(osgEarth::SpatialReference::get("wgs84"),m_unScenePos.fLon,
                                             m_unScenePos.fLat,m_unScenePos.fHeight));
    }
}

/// 初始化样式
void CSceneModel::InitStyle()
{
    m_styleNode.getOrCreate<osgEarth::ModelSymbol>()->autoScale() = true;
    m_styleNode.getOrCreate<osgEarth::ModelSymbol>()->minAutoScale()=1.0;
    m_styleNode.getOrCreate<osgEarth::AltitudeSymbol>()
            ->clamping()=osgEarth::AltitudeSymbol::CLAMP_RELATIVE_TO_TERRAIN;
    m_styleNode.getOrCreate<osgEarth::AltitudeSymbol>()
            ->technique() = osgEarth::AltitudeSymbol::TECHNIQUE_MAP;
    m_styleNode.getOrCreate<osgEarth::AltitudeSymbol>()->binding()
            = osgEarth::AltitudeSymbol::BINDING_VERTEX;
}

void CSceneModel::Set2DAndSamllPic(const string &sPicPth)
{
}

void CSceneModel::SetModelPath(const string &sModelPath)
{
}

void CSceneModel::SetPicModelChange(PIC_MODEL_CHANGE_TYPE, double)
{
}

///
void CSceneModel::SetAutoScal(bool bAutoScal)
{
    if(m_bAutoScal != bAutoScal)
    {
        m_bAutoScal = bAutoScal;
    }
}

void CSceneModel::SetScalBit(double)
{
}

void CSceneModel::SetYPR(double, double, double)
{
}
