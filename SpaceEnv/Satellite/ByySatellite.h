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
     * @brief 设置空间中卫星数据
     * @param rOribit
     */
    void SetOribitData(const std::vector<Math::CVector>& rOribit);

    /**
     * @brief 设置轨道的地理信息
     * @param vOribit
     */
    void SetGeoOribit(const std::vector<Math::CVector>& vOribit);

    /**
     * @brief 放置时间
     * @param rTime
     */
    void SetTime(const std::vector<double>& rTime, double dStart, double dStep, double dEnd);

    /**
     * @brief 更新卫星位置
     * @param dMJD
     */
    void UpdatePos(double dMJD, osg::Matrixd& rMatrix);

    /**
     * @brief 插入覆盖范围
     * @param nID
     * @param rCover
     */
    void InsertCover(int nID, const std::vector<Math::CVector>& rCover);

    /**
     * @brief 设置地面波的显隐状态
     * @param nID
     * @param bVisible
     */
    void SetUnderCover(int nID, bool bVisible);

    /**
     * @brief 设置地面范围是否显示
     * @param bVisible
     */
    void SetUnderEarthVisible(bool bVisible);

    /**
     * @brief 获取地面范围是否显示
     * @return
     */
    bool GetUnderEarthVisible();

    /**
     * @brief 设置圆锥是否可见
     * @param bVisible
     */
    void SetConeVisible(bool bVisible);

    /**
     * @brief 设置圆锥半角
     * @param bAngle
     */
    void SetConeAngle(double dAngle);
    double GetConeAngle();

    /**
     * @brief 设置圆锥角的方位和俯仰
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
     * @brief 获取圆锥显示
     * @return
     */
    bool GetConeVisible();

    /**
     * @brief 获取在地固系下的位置
     * @return
     */
    const osg::Vec3d& GetWorldPos(){return(m_vSatelliteWorldPos);}

    /**
     * @brief 地图更改
     * @param type
     */
    void MapChanged(MAP_TYPE type);

    /**
     * @brief 更新名称
     * @param sName
     */
    void UpdateName(const std::string& sName);

    /**
     * @brief 获取卫星节点
     * @return
     */
    osg::Node* GetSatelliteNode(){return(m_pSatelliteNode.get());}

    void SetConeColor(const osg::Vec4& vColor);

    void Scale(double dScale);

private:
    /**
     * @brief 初始化
     */
    void Init();

    /**
     * @brief 计算当前卫星位置
     */
    void CalNowPos(double dMJD);

    /**
     * @brief 计算插值
     * @param dX
     * @param dT
     * @param nDim
     * @return
     */
    inline double CalItNewton(double* dX, double dT, int nDim);

private:
    QString                      m_sName;
    std::string                   m_sZiTai;
    osg::Group*                m_pRoot;      /// 根节点
    std::vector<Math::CVector> m_vOribit;   /// teme坐标系下的数据
    std::vector<double> m_vdMjd;               /// 坐标对应的时间
    osg::ref_ptr<osg::Node>      m_pSatelliteNode; /// 卫星节点

    osg::ref_ptr<osg::Geometry> m_pOribit;     /// 绘制卫星轨道的节点
    osg::ref_ptr<osg::MatrixTransform> m_pSatellitePos; /// 卫星的位置
    osg::ref_ptr<osg::MatrixTransform> m_pConeZiTai;   /// 波束姿态
    osg::ref_ptr<osg::MatrixTransform> m_pSatellite; /// 卫星模型
    osg::ref_ptr<osgText::Text> text;                              /// 卫星显示的字符


    Math::CVector                           m_stNowPos; /// 当前卫星的位置
    CByyRenderCone*                         m_pCone;     /// 锥形
    CSnSatelliteUnderCover*               m_pUnderCover; /// 地下覆盖范围
    CSnDraw2DSatelliteOribit*             m_p2DOribit;     ///  二维下的轨道
    std::map<int,CSnRenderCone*>    m_mapCoverCon;  /// 地面锥形
    osg::Vec3d                                    m_vSatelliteWorldPos;

    MAP_TYPE                                   m_emMapType;            /// 地图类型

    int                                               m_nIndex;                     /// 当前的索引

    bool                                           m_bUpdateOribit;           /// 轨道是否需要更新
    bool                                           m_bUpdateSatellite;      /// 卫星是否需要更新
    bool                                           m_bCreateUnder;

    double                                        m_dStart;   /// 卫星轨道开始时间
    double                                        m_dEnd;    /// 卫星轨道结束时间
    double                                        m_dStep;   /// 卫星轨道计算步长
    double                                        m_dYaw;   /// 偏航角
    double                                        m_dPitch;  /// 俯仰角
    double                                        m_dRoll;    /// 翻滚角
};

#endif // BYY_SATELLITE_H
