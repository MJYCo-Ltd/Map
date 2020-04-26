#ifndef SNSATELLITEMANAGER_H
#define SNSATELLITEMANAGER_H

#include <QMap>
#include <osg/Matrixd>
#include <osg/Group>
#include <VecMat.h>

class CByySatellite;

/**
 * @brief
 */
class CSnSatelliteManager
{
public:
    CSnSatelliteManager();

    /**
     * @brief 创建卫星
     * @param nID 唯一编号
     * @return
     */
    CByySatellite* GetOrCreateSatellite(const QString& sID);

    /**
     * @brief 更新时间
     * @param dMJD
     * @param matTEME2ECF
     */
    void UpdateTime(double dMJD, osg::Matrixd & matTEME2ECF, CMatrix &rMathMatrix);

    /**
     * @brief 获取节点
     * @return
     */
    osg::Group* GetGroupNode(){return(m_pRoot.get());}

    /**
     * @brief 获取卫星数量
     * @return
     */
    int GetSatelliteCount(){return(m_nSatelliteCount);}


private:
    int                                    m_nSatelliteCount; /// 卫星数量
    QMap<QString,CByySatellite*> m_mapSatellite;  /// 卫星集合
    osg::ref_ptr<osg::Group>           m_pRoot;       /// 没有旋转的根节点
    osg::ref_ptr<osg::MatrixTransform> m_pGroupNode;  /// 根节点
};

#endif // SNSATELLITEMANAGER_H
