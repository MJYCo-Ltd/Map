#include <osg/Callback>
#include <GisMath/GisMath.h>
#include <Sofa/sofam.h>
#include <ISceneCore.h>
#include <Inner/ILoadResource.h>
#include "SceneModel.h"
/// 图片节点
class ImageCallBack:public osg::Callback
{
public:
    ImageCallBack(CSceneModel* pModel):m_pModel(pModel)
    {
    }

    bool run(osg::Object* object, osg::Object* data)
    {
        if(m_pModel->m_bUpdateImage)
        {
            m_pModel->m_bUpdateImage=false;
            m_pModel->m_pImage->setIconImage(m_pModel->m_pPic);
        }

        if(m_pModel->m_bUpdateName)
        {
            m_pModel->m_bUpdateName = false;
            m_pModel->m_pImage->setText(m_pModel->m_sName);
        }

        if(m_pModel->m_bUpdateStle)
        {
            m_pModel->m_bUpdateStle=false;
            m_pModel->m_pImage->setStyle(m_pModel->m_stylePic);
        }

        if(m_pModel->m_bUpdateImagePos)
        {
            m_pModel->m_pImage->setPosition(m_pModel->m_stEarthGeoPoint);
            m_pModel->m_bUpdateImagePos = false;
        }

        return traverse(object, data);
    }
private:
    CSceneModel* m_pModel;
};

/// 模型回调
class ModelCallBack:public osg::Callback
{
public:
    ModelCallBack(CSceneModel* pModel):m_pModel(pModel)
    {
    }

    bool run(osg::Object* object, osg::Object* data)
    {
        if(m_pModel->m_bUpdateModel)
        {
            m_pModel->m_bUpdateModel=false;
            m_pModel->m_pModel->setStyle(m_pModel->m_styleNode);
        }

        if(m_pModel->m_bUpdateModePos)
        {
            m_pModel->m_pModel->setPosition(m_pModel->m_stEarthGeoPoint);
            m_pModel->m_bUpdateModePos = false;
        }

        return traverse(object, data);
    }
private:
    CSceneModel* m_pModel;
};

CSceneModel::CSceneModel(ISceneGraph *pSceneGraph):
    QtDrawShape<IModel>(pSceneGraph)
{
    m_stEarthGeoPoint.set(osgEarth::SpatialReference::get("wgs84"),
                              m_stScenePos.fLon,m_stScenePos.fLat,m_stScenePos.fHeight,
                              osgEarth::AltitudeMode::ALTMODE_RELATIVE);
    m_prePos.bIsGeo = false;
}

/// 设置显示名称
void CSceneModel::SetName(const string &sName)
{
    if(m_sName != sName)
    {
        m_sName = sName;
        m_bUpdateName = true;
    }
}

/// 更新地图节点
void CSceneModel::UpdateMapNode(osgEarth::MapNode *pMapNode)
{
    m_pOsgNode->removeChildren(0,m_pOsgNode->getNumChildren());

    m_pImage->setMapNode(pMapNode);
    if(pMapNode->isGeocentric())
    {
        m_pOsgNode->addChild(m_pLodNode);
        if(!m_pModel.valid())
        {
            m_pModel = new osgEarth::ModelNode(pMapNode,m_styleNode);
            m_pModel->setDynamic(true);
            m_pModel->addUpdateCallback(new ModelCallBack(this));

            /// 修改位置
            PosChanged();

            m_pLodNode->addChild(m_pModel,0.f,m_dChangePos);
        }
        else
        {
            m_pModel->setHorizonCulling(true);
            m_pImage->setHorizonCulling(true);
        }
    }
    else
    {
        m_pOsgNode->addChild(m_pImage);
    }
}

/// 初始化节点
void CSceneModel::InitSceneNode()
{
    QtDrawShape<IModel>::InitSceneNode();

    m_pLodNode = new osg::LOD;
    ChangeLodType();
    m_pOsgNode->addChild(m_pLodNode);

    /// 创建地图
    m_pImage = new osgEarth::PlaceNode("hello",m_stylePic);
    m_pLodNode->addChild(m_pImage,m_dChangePos,FLT_MAX);
    m_pImageCallBack = new ImageCallBack(this);
    m_pImage->addUpdateCallback(m_pImageCallBack);
    m_pImage->setDynamic(true);
}

/// 位置更新
void CSceneModel::PosChanged()
{
    if(!m_stScenePos.bIsGeo) return;
    m_bUpdateImagePos = true;
    m_bUpdateModePos = true;

//    /// 如果是经纬度信息
//    if(m_prePos.bIsGeo && m_prePos != m_stScenePos)
//    {
//        double dAzim1,dAzeim2,dDis;
//        if(0 == GisMath::CalBaiserF(m_prePos.fLon*DD2R,m_prePos.fLat*DD2R,
//                                    m_stScenePos.fLon*DD2R,m_stScenePos.fLat*DD2R
//                                    ,dAzim1,dAzeim2,dDis))
//        {
//            SetYPR(dAzim1 * DR2D,m_dRoll,m_dPitch);
//        }
//    }

    m_stEarthGeoPoint.x() = m_stScenePos.fLon;
    m_stEarthGeoPoint.y() = m_stScenePos.fLat;
    m_stEarthGeoPoint.alt() = m_stScenePos.fHeight;

    m_prePos = m_stScenePos;
}

/// 初始化样式
void CSceneModel::InitStyle()
{
    /// 设置模型的样式
    m_styleNode.getOrCreate<osgEarth::AltitudeSymbol>()
            ->clamping()=osgEarth::AltitudeSymbol::CLAMP_RELATIVE_TO_TERRAIN;
    m_styleNode.getOrCreate<osgEarth::AltitudeSymbol>()
            ->technique() = osgEarth::AltitudeSymbol::TECHNIQUE_SCENE;
    m_styleNode.getOrCreate<osgEarth::AltitudeSymbol>()->binding()
            = osgEarth::AltitudeSymbol::BINDING_CENTROID;

    /// 设置图片的样式
    m_stylePic.getOrCreate<osgEarth::IconSymbol>()->heading()=m_dYaw+m_dInitAngle;
    m_stylePic.getOrCreate<osgEarth::IconSymbol>()->alignment() = osgEarth::IconSymbol::ALIGN_RIGHT_CENTER;
    m_stylePic.getOrCreateSymbol<osgEarth::TextSymbol>()->encoding() = osgEarth::TextSymbol::ENCODING_UTF8;
    m_stylePic.getOrCreateSymbol<osgEarth::TextSymbol>()->alignment() = osgEarth::TextSymbol::ALIGN_LEFT_CENTER;
    m_stylePic.getOrCreateSymbol<osgEarth::TextSymbol>()->size() = 20;
    string sFontPath = GetExePath();
    sFontPath += "fonts/msyh.ttf";
    m_stylePic.getOrCreateSymbol<osgEarth::TextSymbol>()->font() = sFontPath;
    m_stylePic.getOrCreateSymbol<osgEarth::TextSymbol>()->fill()->color() = osgEarth::Color::White;
    m_stylePic.getOrCreateSymbol<osgEarth::TextSymbol>()->halo()->color() = osgEarth::Color::Black;
}

/// 设置二维的图片
void CSceneModel::Set2DAndSamllPic(const string &sPicPth, double dInitAngle)
{
    if(m_sPicPath != sPicPth)
    {
        m_dInitAngle = dInitAngle;
        auto pImage = m_pSceneGraph->ResouceLoader()->LoadImage(sPicPth,32,32);
        if(nullptr != pImage)
        {
            m_sPicPath = sPicPth;
            m_pPic = pImage;
            m_bUpdateImage=true;
        }
    }

    /// 设置角度
    if(fabs(m_dInitAngle -dInitAngle)>1e-15)
    {
        m_dInitAngle = dInitAngle;
        m_stylePic.getOrCreate<osgEarth::IconSymbol>()->heading()=m_dYaw+m_dInitAngle;
        m_bUpdateStle = true;
    }
}

/// 设置模型路径
void CSceneModel::SetModelPath(const string &sModelPath)
{
    if(m_sModelPath != sModelPath)
    {
        auto pNode = m_pSceneGraph->ResouceLoader()->LoadNode(sModelPath);
        if(nullptr != pNode)
        {
            m_sModelPath = sModelPath;
            m_styleNode.getOrCreate<osgEarth::ModelSymbol>()->setModel(pNode);
            m_bUpdateModel = true;
        }
    }
}

/// 设置模型更改类型
void CSceneModel::SetPicModelChange(PIC_MODEL_CHANGE_TYPE changeType, double dDistance)
{
    if(m_emType != changeType)
    {
        m_emType = changeType;
        ChangeLodType();
    }

    if(dDistance > 0 && !osg::equivalent(dDistance,m_dChangePos))
    {
        ChangeLodDistance();
    }
}

/// 设置自动缩放
void CSceneModel::SetAutoScal(bool bAutoScal)
{
    if(m_bAutoScal != bAutoScal)
    {
        m_bAutoScal = bAutoScal;
        m_styleNode.getOrCreate<osgEarth::ModelSymbol>()->autoScale()=m_bAutoScal;
        m_bUpdateModel = true;
    }
}

/// 设置缩放系数
void CSceneModel::SetScalBit(double dScal)
{
    if(dScal >0 && fabs(m_dScalBit - dScal) > 1e-10)
    {
        m_dScalBit = dScal;
        ChangeScal();
    }
}

/// 设置旋转角度
void CSceneModel::SetYPR(double dYaw, double dRoll, double dPitch)
{
    bool bUpdate = false;
    if(fabs(m_dYaw - dYaw) > 1e-6)
    {
        m_dYaw = dYaw;
        bUpdate = true;
    }

    if(fabs(m_dRoll - dRoll) > 1e-6)
    {        m_dRoll = dRoll;
        bUpdate = true;
    }

    if(fabs(m_dPitch - dPitch) > 1e-6)
    {
        m_dPitch = dPitch;
        bUpdate = true;
    }

    if(bUpdate)
    {
        ChangeYPR();
    }
}

/// 更改lod类型
void CSceneModel::ChangeLodType()
{
    switch(m_emType)
    {
    case EYE_DISTANCE:
        m_pLodNode->setRangeMode(osg::LOD::DISTANCE_FROM_EYE_POINT);
        break;
    case MODEL_PIXEL:
        m_pLodNode->setRangeMode(osg::LOD::PIXEL_SIZE_ON_SCREEN);
        break;
    }
}

void CSceneModel::ChangeLodDistance()
{
    m_pLodNode->setRange(0,m_dChangePos,FLT_MAX);
    m_pLodNode->setRange(1,0,m_dChangePos);
}

/// 更改缩放
void CSceneModel::ChangeScal()
{
    m_styleNode.getOrCreate<osgEarth::ModelSymbol>()->scaleX()=m_dScalBit;
    m_styleNode.getOrCreate<osgEarth::ModelSymbol>()->scaleY()=m_dScalBit;
    m_styleNode.getOrCreate<osgEarth::ModelSymbol>()->scaleZ()=m_dScalBit;
    m_bUpdateModel = true;
}

/// 更改姿态角
void CSceneModel::ChangeYPR()
{
    if(fabs(m_dYaw-259.785) < 1e-3)
    {
        return;
    }

    std::cout<<m_dPitch<<'\t'<<m_dRoll<<'\t'<<m_dYaw<<std::endl;
    m_stylePic.getOrCreate<osgEarth::IconSymbol>()->heading()=m_dYaw+m_dInitAngle;
    m_bUpdateStle=true;

    m_styleNode.getOrCreate<osgEarth::ModelSymbol>()->pitch()=m_dPitch;
    m_styleNode.getOrCreate<osgEarth::ModelSymbol>()->roll()=m_dRoll;
    m_styleNode.getOrCreate<osgEarth::ModelSymbol>()->heading()=-m_dYaw;
    m_bUpdateModel = true;
}
