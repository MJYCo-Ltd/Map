#ifndef BYY_SATELLITE_H
#define BYY_SATELLITE_H

#include <vector>
#include <VecMat.h>
#include <osg/Geometry>
#include <osgText/Text>
#include <QString>

#include "IMapEncapsulation.h"

class CByyRenderCone;
class CSnRenderCone;
class SatelliteUpdateCallBack;
class OribitUpdateCallBack;
class CSnSatelliteUnderCover;
class CSnDraw2DSatelliteOribit;

class CByySatellite:public osg::Group
{
public:

    friend class SatelliteUpdateCallBack;
    friend class OribitUpdateCallBack;

    CByySatellite(osg::Group* pRoot);
    ~CByySatellite();

    /**
     * @brief SetModelName
     * @param sModelName
     */
    void SetModelName(const std::string &sModelName);

    /**
     * @brief ���ÿռ�����������
     * @param rOribit
     */
    void SetOribitData(const std::vector<Math::CVector>& rOribit);

    /**
     * @brief ���ù���ĵ�����Ϣ
     * @param vOribit
     */
    void SetGeoOribit(const std::vector<Math::CVector>& vOribit);

    /**
     * @brief ����ʱ��
     * @param rTime
     */
    void SetTime(const std::vector<double>& rTime, double dStart, double dStep, double dEnd);

    /**
     * @brief ��������λ��
     * @param dMJD
     */
    void UpdatePos(double dMJD, osg::Matrixd& rMatrix);

    /**
     * @brief ���븲�Ƿ�Χ
     * @param nID
     * @param rCover
     */
    void InsertCover(int nID, const std::vector<Math::CVector>& rCover);

    /**
     * @brief ���õ��沨������״̬
     * @param nID
     * @param bVisible
     */
    void SetUnderCover(int nID, bool bVisible);

    /**
     * @brief ���õ��淶Χ�Ƿ���ʾ
     * @param bVisible
     */
    void SetUnderEarthVisible(bool bVisible);

    /**
     * @brief ��ȡ���淶Χ�Ƿ���ʾ
     * @return
     */
    bool GetUnderEarthVisible();

    /**
     * @brief ����Բ׶�Ƿ�ɼ�
     * @param bVisible
     */
    void SetConeVisible(bool bVisible);

    /**
     * @brief ����Բ׶���
     * @param bAngle
     */
    void SetConeAngle(double dAngle);
    double GetConeAngle();

    /**
     * @brief ����Բ׶�ǵķ�λ�͸���
     * @param dA
     * @param dZ
     */
    void UpdateZiTai();

    void SetPitchValue(double dP);
    double GetPitchValue(){return(m_dPitch);}

    void SetYawValue(double dYaw);
    double GetYawValue(){return(m_dYaw);}

    void SetRollValue(double dRoll);
    double GetRollValue(){return(m_dRoll);}

    void SetZiTai(std::string& sZiTai);
    std::string GetZiTai(){return m_sZiTai;}

    /**
     * @brief ��ȡԲ׶��ʾ
     * @return
     */
    bool GetConeVisible();

    /**
     * @brief ��ȡ�ڵع�ϵ�µ�λ��
     * @return
     */
    const osg::Vec3d& GetWorldPos(){return(m_vSatelliteWorldPos);}

    /**
     * @brief ��ͼ����
     * @param type
     */
    void MapChanged(MAP_TYPE type);

    /**
     * @brief ��������
     * @param sName
     */
    void UpdateName(const std::string& sName);

    /**
     * @brief ��ȡ���ǽڵ�
     * @return
     */
    osg::Node* GetSatelliteNode(){return(m_pSatelliteNode.get());}

    void SetConeColor(const osg::Vec4& vColor);

    void Scale(double dScale);

private:
    /**
     * @brief ��ʼ��
     */
    void Init();

    /**
     * @brief ���㵱ǰ����λ��
     */
    void CalNowPos(double dMJD);

    /**
     * @brief �����ֵ
     * @param dX
     * @param dT
     * @param nDim
     * @return
     */
    inline double CalItNewton(double* dX, double dT, int nDim);

private:
    QString                      m_sName;
    std::string                   m_sZiTai;
    osg::Group*                m_pRoot;      /// ���ڵ�
    std::vector<Math::CVector> m_vOribit;   /// teme����ϵ�µ�����
    std::vector<double> m_vdMjd;               /// �����Ӧ��ʱ��
    osg::ref_ptr<osg::Node>      m_pSatelliteNode; /// ���ǽڵ�

    osg::ref_ptr<osg::Geometry> m_pOribit;     /// �������ǹ���Ľڵ�
    osg::ref_ptr<osg::MatrixTransform> m_pSatellitePos; /// ���ǵ�λ��
    osg::ref_ptr<osg::MatrixTransform> m_pConeZiTai;   /// ������̬
    osg::ref_ptr<osg::MatrixTransform> m_pSatellite; /// ����ģ��
    osg::ref_ptr<osgText::Text> text;                              /// ������ʾ���ַ�


    Math::CVector                           m_stNowPos; /// ��ǰ���ǵ�λ��
    CByyRenderCone*                         m_pCone;     /// ׶��
    CSnSatelliteUnderCover*               m_pUnderCover; /// ���¸��Ƿ�Χ
    CSnDraw2DSatelliteOribit*             m_p2DOribit;     ///  ��ά�µĹ��
    std::map<int,CSnRenderCone*>    m_mapCoverCon;  /// ����׶��
    osg::Vec3d                                    m_vSatelliteWorldPos;

    MAP_TYPE                                   m_emMapType;            /// ��ͼ����

    int                                               m_nIndex;                     /// ��ǰ������

    bool                                           m_bUpdateOribit;           /// ����Ƿ���Ҫ����
    bool                                           m_bUpdateSatellite;      /// �����Ƿ���Ҫ����
    bool                                           m_bCreateUnder;

    double                                        m_dStart;   /// ���ǹ����ʼʱ��
    double                                        m_dEnd;    /// ���ǹ������ʱ��
    double                                        m_dStep;   /// ���ǹ�����㲽��
    double                                        m_dYaw;   /// ƫ����
    double                                        m_dPitch;  /// ������
    double                                        m_dRoll;    /// ������
};

#endif // BYY_SATELLITE_H
