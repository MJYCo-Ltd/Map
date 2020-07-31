#ifndef STARRENDER_H
#define STARRENDER_H
#include <osg/Drawable>

#include "StarZoneArrayRenderer.h"
#include "StarToneReproducer.h"
#include "Frustum.h"

class ISceneGraph;

class CStarRender:public osg::Drawable
{
    struct StarsRenderContext
    {
        CStarRender *pBuilder;
        osg::RenderInfo *pOsgRenderInfo;
    };

public:
    CStarRender(ISceneGraph* pSceneGraph,float fShowMaxMag);

    /// 渲染回调
    static int renderFunc(int level, unsigned int index, const osg::Vec3 &c0,
            const osg::Vec3 &c1, const osg::Vec3 &c2, void *context, int value);

    /**
     * @brief 设置网格
     * @param vZoneData
     * @param pGeodesicGrid
     * @param nMaxLevel
     */
    void setGeodesicGrid(ZoneArrayVector &vZoneData, GeodesicGrid* pGeodesicGrid
                         , int nMaxLevel, const QMap<int, QString>& mapStarName, osg::Group* pRoot, double dJD);

    /**
     * @brief 重新渲染
     * @param renderInfo
     */
    void drawImplementation(osg::RenderInfo& renderInfo) const;

protected:
    ~CStarRender();
private:
    void init();
    void updateEye(float fov);
    void setupState();
private:
    ISceneGraph*                       m_pSceneGraph;
    ZoneArrayRenderVector                           m_vRenderVector;

    CStarToneReproducer m_StarTR;   /// 更改星星颜色
    Frustum                     m_Frustum; /// 更新裁剪体

    int                   m_nMaxLevel;
    float                m_fShowMaxMag;  /// 最大的视星等
    GeodesicGrid* m_pGeodesicGrid;
};

#endif // STARRENDER_H
