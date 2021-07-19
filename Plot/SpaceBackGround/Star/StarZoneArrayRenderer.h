#ifndef STARZONEARRAYRENDERER_H
#define STARZONEARRAYRENDERER_H
#include <QMap>
#include <osg/Referenced>
#include <osgEarth/Text>

#include "StarZoneArrayBase.h"
class GeodesicGrid;
class ISceneGraph;

class CStarZoneArrayRenderer : public osg::Referenced
{
    struct ZoneInitContext
    {
        CStarZoneArrayRenderer *render;
        IStarZoneArray *za;
    };

public:
    CStarZoneArrayRenderer(ISceneGraph* pSceneGraph,IStarZoneArray *za, double JD
                           , GeodesicGrid* pGrid, const QMap<int,QString>& hip2Names);

    /**
     * @brief 获取级别
     * @return
     */
    int getLevel(){return(m_nLevel);}


    void readZones(IStarZoneArray *za, unsigned int idx, const osg::Vec3f &c0,
            const osg::Vec3f &c1, const osg::Vec3f &c2);
    osg::Geometry *getGeometry(float maxMag, unsigned int idx);
    const std::vector<osg::ref_ptr<osgEarth::Text> >& getStarNames(){return(m_vStarNames);}

    static bool initFunc(int lev, unsigned int index, const osg::Vec3f &c0,
            const osg::Vec3f &c1, const osg::Vec3f &c2, void *context);
private:
    ISceneGraph*        m_pSceneGraph;
    int m_nLevel;
    int m_nStartCount;
    float m_fMinMag;   /// 最小视等;
    double  m_dJD;
    const QMap<int, QString>&                       m_mapHip2Names;
    std::vector<osg::ref_ptr<osgEarth::Text> > m_vStarNames;/// 星星的名字
    std::vector<osg::ref_ptr<osg::Geometry> > m_vStar;         /// 星星的点
};

typedef std::vector<osg::ref_ptr<CStarZoneArrayRenderer> > ZoneArrayRenderVector;
#endif // STARZONEARRAYRENDERER_H
