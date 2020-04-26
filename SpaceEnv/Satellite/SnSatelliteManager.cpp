#include <osg/MatrixTransform>
#include "ByySatellite.h"
#include "SnSatelliteManager.h"

CSnSatelliteManager::CSnSatelliteManager()
    :m_nSatelliteCount(0)
{
    m_pRoot = new osg::Group;

    m_pGroupNode = new osg::MatrixTransform;
    m_pRoot->addChild(m_pGroupNode);
}

/// 添加卫星
CByySatellite *CSnSatelliteManager::GetOrCreateSatellite(const QString &sID)
{
    QMap<QString,CByySatellite*>::iterator itor = m_mapSatellite.find(sID);
    if(m_mapSatellite.end() != itor)
    {
        return(itor.value());
    }
    else
    {
        CByySatellite* pSatellite = new CByySatellite(m_pRoot);

        /// 保存信息到 map中
        m_mapSatellite[sID] = pSatellite;
        m_pGroupNode->addChild(pSatellite);
        pSatellite->setName(sID.toLocal8Bit().data());
        pSatellite->SetConeVisible(false);


        ++m_nSatelliteCount;

        return(pSatellite);
    }
}

/// 更新时间
void CSnSatelliteManager::UpdateTime(double dMJD, osg::Matrixd &matTEME2ECF,Math::CMatrix& rMathMatrix)
{
    QMap<QString,CByySatellite*>::iterator itor = m_mapSatellite.begin();

    m_pGroupNode->setMatrix(matTEME2ECF);
    /// 更新位置
    while(m_mapSatellite.end() != itor)
    {
        itor.value()->UpdatePos(dMJD,matTEME2ECF);
        ++itor;
    }
}
