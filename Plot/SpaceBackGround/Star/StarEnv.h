#ifndef SA_STAR_ENV_H
#define SA_STAR_ENV_H

#include <osgViewer/View>
#include <Math/Matrix.h>

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

    /**
     * @brief 设置银河的显隐
     */
    void SetMilkwayVisible(bool);

    /**
     * @brief 更新矩阵
     * @param rRotate
     */
    void UpdateMatrix(const Math::CMatrix &rRotate);

protected:
    void InitProjMatrix();
    ~CStarEnv();
private:
    bool                               m_bInit{false};
    bool                               m_bRotate{true};
    osg::Matrix                        m_rRoate;
    ISceneGraph*                       m_pSceneGraph;
    osg::ref_ptr<CMilkyway>            m_pMilkyway;
    osg::ref_ptr<CBoundary>            m_pBoundary;
    osg::ref_ptr<CStarManager>         m_pStarManager;
    osg::observer_ptr<osgViewer::View> m_pMainView;
    osg::observer_ptr<osg::Camera>     m_pMainCamera;
};

#endif // SA_STAR_ENV_H
