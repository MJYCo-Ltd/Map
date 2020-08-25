#include <osg/Depth>
#include "ScenePulse.h"

/// 脉冲波的回调函数
class CPulseCallback:public osg::Callback
{
public:
    CPulseCallback(CScenePulse* pParent):m_pParent(pParent)
    {
    }

    /// 初始化
    static void Init()
    {
        if(m_bInit)
        {
            return;
        }

        m_bInit = true;
        for(int nIndex=0; nIndex<361; ++nIndex)
        {
            s_dXY[nIndex][0] = cos(osg::DegreesToRadians((double)nIndex));
            s_dXY[nIndex][1] = sin(osg::DegreesToRadians((double)nIndex));
        }
    }

    /// 重写父类
    bool run(osg::Object* object, osg::Object* data)
    {

        /// 如果转换成功
        {
            m_pVec3Array->clear();
            m_pVec4Array->at(0).set(m_pParent->m_stColor.fR,m_pParent->m_stColor.fG,
                                    m_pParent->m_stColor.fB,m_pParent->m_stColor.fA);

            double dMin,dMax,dMinR,dMaxR;
            /// 绘制第一个
            if(m_dFirst > m_pParent->m_dSpace)
            {
                m_dFirst = fmod(m_dFirst,m_pParent->m_dSpace);
            }

            if(m_dFirst < m_pParent->m_dHalSpace)
            {
                dMin = 0;
                dMax = m_dFirst;
            }
            else
            {
                dMin = m_dFirst - m_pParent->m_dHalSpace;
                dMax = m_dFirst;
            }

            dMinR = m_pParent->m_dStartRadius + dMin * m_pParent->m_dSpaceRadius;
            dMaxR = m_pParent->m_dStartRadius + dMax * m_pParent->m_dSpaceRadius;

            for(int nIndexj=0; nIndexj<361; ++nIndexj)
            {
                m_pVec3Array->push_back(osg::Vec3(s_dXY[nIndexj][0]*dMinR,s_dXY[nIndexj][1]*dMinR,dMin));
                m_pVec3Array->push_back(osg::Vec3(s_dXY[nIndexj][0]*dMaxR,s_dXY[nIndexj][1]*dMaxR,dMax));
            }

            /// 绘制中间的
            for(int nIndexi=0; nIndexi < m_pParent->m_nPulseCount; ++nIndexi)
            {
                dMin = m_pParent->m_dSpace * nIndexi + m_pParent->m_dHalSpace + m_dFirst;
                dMax = m_pParent->m_dSpace *(nIndexi + 1) + m_dFirst;

                dMinR = m_pParent->m_dStartRadius + dMin * m_pParent->m_dSpaceRadius;
                dMaxR = m_pParent->m_dStartRadius + dMax * m_pParent->m_dSpaceRadius;

                for(int nIndexj=0; nIndexj<361; ++nIndexj)
                {
                    m_pVec3Array->push_back(osg::Vec3(s_dXY[nIndexj][0]*dMinR,s_dXY[nIndexj][1]*dMinR,dMin));
                    m_pVec3Array->push_back(osg::Vec3(s_dXY[nIndexj][0]*dMaxR,s_dXY[nIndexj][1]*dMaxR,dMax));
                }
            }

            /// 绘制最后一个
            if(m_dFirst < m_pParent->m_dHalSpace)
            {
                dMin += m_pParent->m_dSpace;
                dMax = dMin + m_pParent->m_dHalSpace - m_dFirst;
                dMinR = m_pParent->m_dStartRadius + dMin * m_pParent->m_dSpaceRadius;
                dMaxR = m_pParent->m_dStartRadius + dMax * m_pParent->m_dSpaceRadius;

                for(int nIndexj=0; nIndexj<361; ++nIndexj)
                {
                    m_pVec3Array->push_back(osg::Vec3(s_dXY[nIndexj][0]*dMinR,s_dXY[nIndexj][1]*dMinR,dMin));
                    m_pVec3Array->push_back(osg::Vec3(s_dXY[nIndexj][0]*dMaxR,s_dXY[nIndexj][1]*dMaxR,dMax));
                }
            }

            m_pDrawArray->setCount(m_pVec3Array->size());
            m_pDrawArray->dirty();
            m_pParentDraw->dirtyGLObjects();

            m_dFirst += m_dAddSpace;
        }

        return traverse(object, data);
    }

    void SetVec3Array(osg::Vec3Array* pArray){m_pVec3Array = pArray;}
    void SetColorArray(osg::Vec4Array* pColor){m_pVec4Array = pColor;}
    void SetDrawArrays(osg::DrawArrays* pDrawArrays){m_pDrawArray = pDrawArrays;}
    void SetGeometry(osg::Geometry* pDrawGeo){m_pParentDraw = pDrawGeo;}

private:
    osg::Vec3Array* m_pVec3Array;
    osg::Vec4Array* m_pVec4Array;
    osg::DrawArrays* m_pDrawArray;
    osg::Geometry*   m_pParentDraw;
    CScenePulse*     m_pParent;
    double           m_dFirst=0;
    double           m_dAddSpace = 100;

    static double s_dXY[360][2];
    static  bool   m_bInit;
};

double CPulseCallback::s_dXY[360][2] = {{},{}};
bool     CPulseCallback::m_bInit = false;

CScenePulse::CScenePulse(ISceneGraph *pSceneGraph):
    QtSensor<IPulseSensor>(pSceneGraph)
{
    CPulseCallback::Init();
}

void CScenePulse::SetStartRadius(const double & dRadius)
{
    m_dStartRadius = dRadius;
}

void CScenePulse::SetEndRadius(const double &dRaius)
{
    m_dEndRadius = dRaius;
}

void CScenePulse::SetPulseCount(const int &nCount)
{
    m_nPulseCount = nCount;
}

void CScenePulse::SetEndPos(const ScenePos &rPos)
{
    if(m_stEndPos != rPos)
    {
        m_stEndPos = rPos;
        if(m_stScenePos.bIsGeo)
        {
            osg::Vec3d vEnd2Start;
            if(m_stEndPos.bIsGeo)
            {
                osgEarth::GeoPoint geoPos(osgEarth::SpatialReference::get("wgs84"),
                                          m_stEndPos.fLon,m_stEndPos.fLat,m_stEndPos.fHeight,
                                          osgEarth::AltitudeMode::ALTMODE_RELATIVE);
                osg::Matrixd local2World;
                geoPos.createLocalToWorld(local2World);


                vEnd2Start = local2World.getTrans()-m_pUpdataCall->Pos();
            }
            else
            {
                osg::Vec3d vPos(m_stEndPos.fX,m_stEndPos.fY,m_stEndPos.fZ);
                vEnd2Start = vPos - m_pUpdataCall->Pos();
            }
            m_dLength = vEnd2Start.length();
            m_dSpace = m_dLength / m_nPulseCount;
            m_dHalSpace = m_dSpace / 2.0;
            m_dSpaceRadius = (m_dEndRadius - m_dStartRadius)/m_dLength;

            /// 计算旋转矩阵
            osgEarth::GeoPoint geoPos(osgEarth::SpatialReference::get("wgs84"),
                                      m_stScenePos.fLon,m_stScenePos.fLat,m_stScenePos.fHeight,
                                      osgEarth::AltitudeMode::ALTMODE_RELATIVE);
            osg::Matrixd local2World;
            geoPos.createLocalToWorld(local2World);

            osg::Vec3d vTemp = local2World.preMult(vEnd2Start);

            /// 根据叉乘 计算法向量
            osg::Vec3d vAxis = osg::Vec3d(0,0,1) ^ vTemp;

            /// 根据点乘 计算旋转角度
            double dDot = osg::Vec3d(0,0,1) * vTemp;
            double dAngle =acos(dDot/m_dLength);

            vAxis.normalize();

            /// 构建旋转
            osg::Quat tmpRotate(dAngle,vAxis);

            local2World(3,0) = local2World(3,1) = local2World(3,2)=0;
            local2World.preMultRotate(tmpRotate);
            m_pUpdataCall->SetMatrix(local2World);
        }
    }
}

void CScenePulse::InitSceneNode()
{
    QtSensor<IPulseSensor>::InitSceneNode();

    auto pLocalGeometry = new osg::Geometry;
    pLocalGeometry->setDataVariance(osg::Object::DYNAMIC);
    /// 开启颜色混合 关闭光照
    pLocalGeometry->getOrCreateStateSet()->setMode(GL_BLEND,osg::StateAttribute::ON);
    pLocalGeometry->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

    /// 写深度缓存
    osg::Depth* pDepth = new osg::Depth;
    pDepth->setWriteMask(false);
    pLocalGeometry->getOrCreateStateSet()->setAttribute(pDepth);

    osg::ref_ptr<osg::Vec3Array> pArray = new osg::Vec3Array;
    osg::ref_ptr<osg::Vec4Array> pColorArray = new osg::Vec4Array;
    pColorArray->push_back(osg::Vec4(m_stColor.fR,m_stColor.fG,m_stColor.fB,m_stColor.fA));

    pLocalGeometry->setVertexArray(pArray.get());
    pLocalGeometry->setColorArray(pColorArray.get(),osg::Array::BIND_OVERALL);

    osg::DrawArrays* pDrawArray = new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP,0,pArray->size());
    pLocalGeometry->addPrimitiveSet(pDrawArray);
    CPulseCallback *pNewPulseCallback = new CPulseCallback(this);
    pLocalGeometry->setUpdateCallback(pNewPulseCallback);
    pNewPulseCallback->SetVec3Array(pArray.get());
    pNewPulseCallback->SetDrawArrays(pDrawArray);
    pNewPulseCallback->SetColorArray(pColorArray);
    pNewPulseCallback->SetGeometry(pLocalGeometry);
    m_pOsgNode->addChild(pLocalGeometry);
}

/// 位置更新
void CScenePulse::PosChanged()
{
    if(m_stScenePos.bIsGeo)
    {
        osgEarth::GeoPoint geoPos(osgEarth::SpatialReference::get("wgs84"),
                                  m_stScenePos.fLon,m_stScenePos.fLat,m_stScenePos.fHeight,
                                  osgEarth::AltitudeMode::ALTMODE_RELATIVE);
        osg::Matrixd local2World;
        geoPos.createLocalToWorld(local2World);


        m_pUpdataCall->SetPos(local2World.getTrans());
        local2World(3,0) = local2World(3,1) = local2World(3,2)=0;
        m_pUpdataCall->SetMatrix(local2World);
    }
    else
    {
        QtSensor<IPulseSensor>::PosChanged();
    }
}
