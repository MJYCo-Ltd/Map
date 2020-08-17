#ifndef CSPACEENV_H
#define CSPACEENV_H
#include <osg/Camera>
#include <Inner/QtOsgSceneNode.h>
#include <SpaceEnv/ISpaceEnv.h>

class CSpaceEnv:public QtOsgSceneNode<ISpaceEnv>
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
    void InitSceneNode();

protected:

    /**
     * @brief 加载空间背景
     */
    void LoadBackGround();
protected:
    ISpaceBackGround* m_pSpaceBackGround=nullptr;
    bool              m_bShowSpaceBackGround=true;
    osg::observer_ptr<osg::Camera> m_pMainCamera;
};

#endif // CSPACEENV_H
