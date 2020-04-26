#ifndef SA_STAR_ENV_H
#define SA_STAR_ENV_H

#include <osgViewer/View>

#include "Milkyway.h"
#include "Boundary.h"
#include "StarManager.h"

class ISceneGraph;

class CStarEnv : public osg::Camera
{
public:
    CStarEnv(ISceneGraph* pSceneGraph);

    /**
     * @brief 设置主view的camera
     * @param pMainCamera
     */
    void SetMainView(osgViewer::View* pMainView);

    /**
     * @brief 更新地惯系矩阵
     * @param crMatrix
     */
    void UpdateMatrix(const osg::Matrix& crMatrix);

    /**
     * @brief 重写父类方法
     * @param nv
     */
    void traverse(osg::NodeVisitor& nv);

    /**
     * @brief 设置星区显示
     * @param bVisible
     */
    void SetBoundaryVisible(bool bVisible);

    /**
     * @brief 设置名称显示
     * @param bVisible
     */
    void SetStarNameVisible(bool bVisible);

    /**
     * @brief 设置星座显示
     * @param bVisible
     */
    void SetConstellationVisible(bool bVisible);

protected:

    ~CStarEnv();
private:
    ISceneGraph*                       m_pSceneGraph;
    osg::ref_ptr<CMilkyway>            m_pMilkyway;
    osg::ref_ptr<CBoundary>            m_pBoundary;
    osg::ref_ptr<CStarManager>         m_pStarManager;
    osg::observer_ptr<osgViewer::View> m_pMainView;
    osg::observer_ptr<osg::Camera>     m_pMainCamera;
    osg::Matrix                        m_rRotMatrix;
};

#endif // SA_STAR_ENV_H
