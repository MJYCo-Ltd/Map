#ifndef CSPACEENV_H
#define CSPACEENV_H
#include <osg/Camera>
#include <Inner/IOsgSceneNode.h>
#include <Inner/ImplSceneAttitudeGroup.hpp>
#include <Plot/Map/SpaceEnv/ISpaceEnv.h>
#include <Plot/Common/ISceneAttitudeGroup.h>

class CSpaceEnv:public ImplSceneAttitudeGroup<ISpaceEnv>
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
     * @brief 控制空间背景的显隐
     */
    void ShowSpaceBackGround(bool);

    /**
     * @brief 初始化节点
     */
    void InitNode();

    /**
     * @brief 更新时间
     */
    void UpdateDate(double);

    /**
     * @brief 获取太阳位置
     * @return
     */
    const Math::CVector& GetSunPos(){return(m_vSunPos);}
protected:

    /**
     * @brief 加载空间背景
     */
    void LoadBackGround();
protected:
    ISpaceBackGround* m_pSpaceBackGround=nullptr;
    bool              m_bShowSpaceBackGround=true;
    osg::observer_ptr<osg::Camera> m_pMainCamera;
    Math::CVector     m_vSunPos;
};

#endif // CSPACEENV_H
