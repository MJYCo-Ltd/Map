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
     * @brief ��������
     * @param nID Ψһ���
     * @return
     */
    CByySatellite* GetOrCreateSatellite(const QString& sID);

    /**
     * @brief ����ʱ��
     * @param dMJD
     * @param matTEME2ECF
     */
    void UpdateTime(double dMJD, osg::Matrixd & matTEME2ECF, CMatrix &rMathMatrix);

    /**
     * @brief ��ȡ�ڵ�
     * @return
     */
    osg::Group* GetGroupNode(){return(m_pRoot.get());}

    /**
     * @brief ��ȡ��������
     * @return
     */
    int GetSatelliteCount(){return(m_nSatelliteCount);}


private:
    int                                    m_nSatelliteCount; /// ��������
    QMap<QString,CByySatellite*> m_mapSatellite;  /// ���Ǽ���
    osg::ref_ptr<osg::Group>           m_pRoot;       /// û����ת�ĸ��ڵ�
    osg::ref_ptr<osg::MatrixTransform> m_pGroupNode;  /// ���ڵ�
};

#endif // SNSATELLITEMANAGER_H
