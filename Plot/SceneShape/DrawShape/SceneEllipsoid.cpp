#include <GisMath/GisMath.h>
#include <VersionMathCommon.h>
#include "SceneEllipsoid.h"
#include <Inner/OsgExtern/JudgeGeometry.h>

/// 形状 更新
void CSceneEllipsoid::UpdateShape()
{
    m_pGeometry->removePrimitiveSet(0,m_pGeometry->getNumPrimitiveSets());
    m_pEllipsoidModel->setRadiusEquator(m_dEquator);
    m_pEllipsoidModel->setRadiusPolar(m_dPolar);

    m_pVertexArray->clear();

    double dStartLat(90.),dEndLat(-90),
            dStartLon(-180.),dEndLon(180),
            dLatSegmentSize,dLonSegmentSize;

    int nLatSegments(m_unSegments),nLonSegments(nLatSegments*2);
    osg::Texture2D* pTexture=nullptr;

    switch (m_emDrawType)
    {
    case FULL_PART:
    {
        dLatSegmentSize = (dStartLat-dEndLat)/(double)nLatSegments; // degrees
        dLonSegmentSize = (dEndLon-dStartLon)/(double)nLonSegments; // degrees
        m_pVertexArray->reserve((nLatSegments+1) * (nLonSegments+1));
        m_pNormals->clear();
        m_pTexCoords->clear();

        if(!m_sImagePath.empty())
        {
            pTexture = m_pSceneGraph->ResouceLoader()->LoadTexture(m_sImagePath);
        }

        m_pNormals->clear();
        m_pNormals->reserve(m_pVertexArray->capacity());
        m_pGeometry->setNormalArray(m_pNormals);

        if(nullptr != pTexture)
        {

            m_pTexCoords->clear();

            m_pTexCoords->reserve(m_pVertexArray->capacity());


            m_pGeometry->setTexCoordArray(0,m_pTexCoords);
            auto pNodeState = m_pGeometry->getOrCreateStateSet();

            pNodeState->setTextureAttributeAndModes(0, pTexture,osg::StateAttribute::ON);

            auto pStateSet = m_pSceneGraph->ResouceLoader()->LoadVirtualProgram("GLSL/Global.glsl");
            m_pGeometry->setStateSet(m_pSceneGraph->ResouceLoader()->MergeStateSet(pStateSet,pNodeState));
        }
        else
        {
            m_pGeometry->setTexCoordArray(0,nullptr);
            m_pGeometry->setNormalArray(nullptr);
            m_pGeometry->getOrCreateStateSet()->setTextureAttributeAndModes(0, nullptr,osg::StateAttribute::OFF);

            /// 如果VP中有函数则移除
            m_pSceneGraph->ResouceLoader()->RemoveVirtualProgram("GLSL/Global.glsl",m_pGeometry->getStateSet());
        }
    }
        break;
    case SUB_PART:
        dStartLon=m_dMinAzim;
        dStartLat=m_dMaxElev;
        nLatSegments = (m_dMaxElev-m_dMinElev)/10;
        if(nLatSegments < 1)
        {
            nLatSegments = 1;
        }
        nLonSegments = (m_dMaxAzim-m_dMinAzim)/10;
        if(nLonSegments < 1)
        {
            nLonSegments = 1;
        }

        dLatSegmentSize = (m_dMaxElev-m_dMinElev)/(double)nLatSegments;
        dLonSegmentSize = (m_dMaxAzim-m_dMinAzim)/(double)nLonSegments;

        m_pVertexArray->reserve((nLatSegments+1) * (nLonSegments+1)+1);
        break;
    case CONE_PART:
        dEndLat = dStartLat - m_dAngle;
        nLatSegments = m_dAngle/10;
        if(nLatSegments < 1)
        {
            nLatSegments = 1;
        }
        m_pVertexArray->reserve((nLatSegments+1) * (nLonSegments+1)+1);

        dLatSegmentSize = (dStartLat-dEndLat)/(double)nLatSegments;
        dLonSegmentSize = (dEndLon - dStartLon)/(double)nLonSegments;
        break;
    case SCONE_PART:
    {
        Math::CVector vOut(3);

        nLatSegments = 2*m_dVAngle/10;
        nLonSegments = 2*m_dHAngle/10;

        dStartLat = m_dVAngle;
        dStartLon = -m_dHAngle;

        dLatSegmentSize = 2*m_dVAngle/(double)nLatSegments;
        dLonSegmentSize = 2*m_dHAngle/(double)nLonSegments;
    }
        break;
    }

    double lat,lon,gx,gy,gz,ds,dt;
    int x,y,y_plus_1;
    static Math::CVector vCenter(3);
    Math::CVector vDir(0,0,-1),vOut(3);
    osg::Vec3 normal;

    for( y = 0; y <= nLatSegments; ++y )
    {
        osg::DrawElementsUShort* el = new osg::DrawElementsUShort(GL_TRIANGLE_STRIP);
        el->reserve( 2 * (nLonSegments+1));

        lat = dStartLat - dLatSegmentSize * (double)y;
        for( x = 0; x <= nLonSegments; ++x )
        {
            lon = dStartLon + dLonSegmentSize * (double)x;

            if(m_emDrawType != SCONE_PART)
            {
                m_pEllipsoidModel->convertLatLongHeightToXYZ(lat*DD2R,
                                                             lon*DD2R,
                                                             0., gx, gy, gz );
            }
            else
            {
                vDir.SetX(tan(lon*DD2R));
                vDir.SetY(tan(lat*DD2R));
                GisMath::CalLineInterEllipsoid(vCenter,vDir,vOut,m_dEquator,m_dPolar);
                gx=vOut.GetX();
                gy=vOut.GetY();
                gz=vOut.GetZ();
            }

            m_pVertexArray->push_back(osg::Vec3(gx,gy,gz));

            if(nullptr != pTexture)
            {
                ds = (lon + 180) / 360.0;
                dt = (lat + 90.0) / 180.0;
                m_pTexCoords->push_back(osg::Vec2(ds,dt));
            }

            normal.set(gx, gy, gz);
            normal.normalize();
            m_pNormals->push_back(normal);


            y_plus_1 = y+1;
            if ( y < nLatSegments)
            {
                el->push_back( y*(nLonSegments+1) + x);
                el->push_back( y_plus_1*(nLonSegments+1) + x);
            }
        }
        m_pGeometry->addPrimitiveSet(el);
    }

    if(m_emDrawType != FULL_PART)
    {
        m_pVertexArray->push_back(osg::Vec3());
        if(m_emDrawType == SUB_PART || m_emDrawType==SCONE_PART)
        {
            osg::DrawElementsUShort* el1 = new osg::DrawElementsUShort(GL_TRIANGLE_FAN);
            osg::DrawElementsUShort* el2 = new osg::DrawElementsUShort(GL_TRIANGLE_FAN);
            el1->reserve(nLonSegments+2);
            el2->reserve(nLonSegments+2);

            el1->push_back((nLatSegments+1)*(nLonSegments+1));
            el2->push_back((nLatSegments+1)*(nLonSegments+1));
            for( x=nLonSegments; x>-1; --x )
            {
                el1->push_back(nLonSegments-x);
                el2->push_back(nLatSegments*(nLonSegments+1) + x);
            }

            m_pGeometry->addPrimitiveSet(el1);
            m_pGeometry->addPrimitiveSet(el2);

            if(SUB_PART==m_emDrawType && fabs(m_dMaxAzim-m_dMinAzim-360)>DBL_EPSILON)
            {
                osg::DrawElementsUShort* el3 = new osg::DrawElementsUShort(GL_TRIANGLE_FAN);
                osg::DrawElementsUShort* el4 = new osg::DrawElementsUShort(GL_TRIANGLE_FAN);
                el3->reserve(nLatSegments+2);
                el4->reserve(nLatSegments+2);
                el3->push_back((nLatSegments+1)*(nLonSegments+1));
                el4->push_back((nLatSegments+1)*(nLonSegments+1));
                for( y=nLatSegments; y>-1; --y )
                {
                    el3->push_back(y*(nLonSegments+1));
                    el4->push_back((nLatSegments-y)*(nLonSegments+1) + nLonSegments);
                }

                m_pGeometry->addPrimitiveSet(el3);
                m_pGeometry->addPrimitiveSet(el4);
            }
        }
        else
        {
            osg::DrawElementsUShort* el1 = new osg::DrawElementsUShort(GL_TRIANGLE_FAN);
            el1->reserve(nLonSegments+2);
            el1->push_back((nLatSegments+1)*(nLonSegments+1));
            for(x=nLonSegments;x>-1;--x)
            {
                el1->push_back(nLatSegments*(nLonSegments+1) + x);
            }
            m_pGeometry->addPrimitiveSet(el1);
        }
    }
    else if(nullptr != pTexture)
    {
        m_pTexCoords->dirty();
    }

    m_pNormals->dirty();
}

/// 创建模型
void CSceneEllipsoid::CreateShape()
{
    m_pEllipsoidModel = new osg::EllipsoidModel;
    m_pTexCoords = new osg::Vec2Array(osg::Array::BIND_PER_VERTEX);
    m_pNormals = new osg::Vec3Array(osg::Array::BIND_PER_VERTEX);
    m_pGeometry->setNormalArray(m_pNormals);
    UpdateShape();
}
