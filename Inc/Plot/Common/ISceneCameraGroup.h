#ifndef INTERFACE_SCENE_CAMERA_GROUP_HEARDER_H
#define INTERFACE_SCENE_CAMERA_GROUP_HEARDER_H

#include <SceneGraph/ISceneGroup.h>
/**
 * @brief 自动计算远近裁剪面的节点
 */
class ISceneCameraGroup:public ISceneGroup
{
public:
    CONSTRUCTOR(ISceneCameraGroup,ISceneGroup)

    void SetRenderIndex(int nIndex)JUDGE_EQUAL_CALL_FUNCTION(nIndex,m_nIndex,RenderIndexChanged)
    /**
     * @brief 转换成缩放节点
     * @return
     */
    ISceneCameraGroup* AsSceneCameraGroup(){return(this);}

protected:
    virtual ~ISceneCameraGroup(){}
    virtual void RenderIndexChanged()=0;

protected:
    int m_nIndex=0;
};

#endif//INTERFACE_SCENE_CAMERA_GROUP_HEARDER_H
