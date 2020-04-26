#ifndef STARCONSTELLATION_H
#define STARCONSTELLATION_H
#include<QMap>
#include<QVector>
#include<osgText/Text>

#include "StarZoneArrayRenderer.h"

class DrawConstellation;
class ISceneGraph;

class CStarConstellation
{
public:

    struct FindContext
    {
        ZoneArrayVector* vZoneArray;
        CStarConstellation *pBuilder;
        double dJD;
    };

    static bool findPosition(int lev, unsigned int index, const osg::Vec3f &c0,
                             const osg::Vec3f &c1, const osg::Vec3f &c2, void *context);

    CStarConstellation(ISceneGraph* pSceneGraph);

    /**
     * @brief 读取星座线
     * @return
     */
    bool ReadConstellation();

    /**
     * @brief 设置网格划分
     * @param nLevel
     * @param vZoneData
     * @param pGeodesicGrid
     * @param dJD
     */
    void setGeodesicGrid(int nLevel, ZoneArrayVector& vZoneData, GeodesicGrid* pGeodesicGrid, osg::Group *pRoot, double dJD);

protected:
    void BuildGeometry(osg::Group *pRoot);

private:
    ISceneGraph*                               m_pSceneGraph;
    QMap<QString,QVector<int> >                m_mapName2Hips; /// 星座连线
    QMap<int,osg::Vec3>                        m_mapHip2Pos;   /// 星星的位置

    osg::ref_ptr<DrawConstellation>           m_pConstellationLine;
    std::vector<osg::ref_ptr<osgText::Text> > m_vNames;
};

#endif // STARCONSTELLATION_H
