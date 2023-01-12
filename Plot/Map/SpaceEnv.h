#ifndef CSPACEENV_H
#define CSPACEENV_H
#include <osg/Camera>
#include <Inner/IOsgSceneNode.h>
#include <Inner/Common/ImplSceneGroup.hpp>
#include <Plot/Map/SpaceEnv/ISpaceEnv.h>

class CSpaceEnv:public ImplSceneGroup<ISpaceEnv>
{
public:
    CSpaceEnv(ISceneGraph*);

    /**
     * @brief 设置视口主相机
     * @param pCamera
     */
    void SetMainCamara(osg::Camera* pCamera);

    /**
     * @brief 获取空间背景
     * @return
     */
    ISpaceBackGround* GetSpaceBackGround();

    /**
     * @brief 初始化节点
     */
    void InitNode();

    /**
     * @brief 更新时间
     */
    void UpdateDate(double, const Math::CMatrix &rBackGroundMatrix,const Math::CMatrix& rCaleRote);

    /**
     * @brief 获取太阳位置
     * @return
     */
    const Math::CVector& GetSunPos(){return(m_vSun);}

    /**
     * @brief 重写子类显隐控制
     */
    void VisibleChanged();
protected:
    ISpaceBackGround* m_pSpaceBackGround{};
    bool              m_bShowSpaceBackGround{true};
    osg::observer_ptr<osg::Camera> m_pMainCamera;
    std::vector<Math::CVector> m_vAllPos;
    Math::CVector              m_vSun;
};

#endif // CSPACEENV_H
