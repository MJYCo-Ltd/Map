#ifndef SA_STAR_MANGER_H
#define SA_STAR_MANGER_H

#include <vector>
#include <QString>
#include <QMap>
#include <osg/ref_ptr>
#include <osg/Node>

#include "StarZoneArray.h"

class GeodesicGrid;
class CStarRender;
class CStarConstellation;
class ISceneGraph;

enum TYPE_STAR_DATA
{
    CAT,
    BIN
};

class CStarManager:public osg::Referenced
{
public:
    CStarManager(ISceneGraph* pSceneGraph);
    ~CStarManager();

    /**
     * @brief 读取
     * @param sStarPath
     */
    void ReadStar(int nMaxLevel,TYPE_STAR_DATA type=BIN);

    /**
     * @brief 设置显示的视星等
     */
    void SetShowMaxMag(float fMaxMag);

    /**
     * @brief 获取视星等
     * @return
     */
    float GetShowMaxMag(){return(m_fShowMaxMag);}

    /**
     * @brief 获取渲染节点
     * @return
     */
    osg::Node *getNode();

    /**
     * @brief 显示恒星的显示状态
     * @param bVisible
     */
    void SetStarNameVisible(bool bVisible);

    /**
     * @brief 设置星区的显示状态
     * @param bVisible
     */
    void SetConstellation(bool bVisible);
protected:

    /**
     * @brief 读取星星名字
     * @return
     */
    bool ReadStarNames();

    /**
     * @brief 读取bin文件
     * @param filename
     * @param maxlevel
     * @return
     */
    bool ReadBin(const QString& sFilename, int maxlevel);

    /**
     * @brief 读取cat文件
     * @param csFileName 文件名称
     * @param nMaxLevel  最大等级
     * @return
     */
    bool ReadCat(const QString& sFilename, int nMaxLevel);

    /**
     * @brief 遍历文件夹搜索所有的文件
     * @param sDirectory
     * @param nMaxLevel
     */
    void Directory(const QString& sDirectory, int nMaxLevel);

private:
    ISceneGraph*        m_pSceneGraph;
    ZoneArrayVector     m_vZones;             /// 所有星区信息
    QMap<int, QString>  m_mapHip2Name;        /// 星星的名字

    quint8              m_unMaxLevel;         /// 所有星区最大的级数
    float               m_fShowMaxMag;        /// 最大星等

    bool                m_bShowStarName;      /// 是否显示恒星名称
    bool                m_bShowConstell;      /// 是否显示星座连线
    bool                m_bNeedUpdate;

    GeodesicGrid*       m_pGeodesicGrid;      /// 划分网格类
    CStarRender*        m_pStarRender;        /// 绘制星星类
    CStarConstellation* m_pStarConstellation; /// 绘制星座线

    osg::ref_ptr<osg::Group> m_pRoot;         /// 根节点
    osg::ref_ptr<osg::Group> m_pStarName;     /// 恒星名字
    osg::ref_ptr<osg::Group> m_pConstell;     /// 星座
};

#endif // SA_STAR_MANGER_H
