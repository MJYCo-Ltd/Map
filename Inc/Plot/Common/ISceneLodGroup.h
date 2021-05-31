#ifndef INTERFACE_SCENE_LOD_GROUP_HEARDER_H
#define INTERFACE_SCENE_LOD_GROUP_HEARDER_H
#include <vector>
#include <SceneGraph/ISceneGroup.h>


/**
 * @brief 场景切换节点，根据需要切换不同的显示
 */
class ISceneLodGroup:public ISceneGroup
{
public:
    enum SCENE_NODE_CHANGE_TYPE
    {
        NODE_EYE_DISTANCE, ///距离视点的距离
        NODE_SCREEN_PIXEL  ///模型在屏幕上的像素大小(此方法对自动缩放的模型无效)
    };

    CONSTRUCTOR(ISceneLodGroup,ISceneGroup)

    /**
     * @brief 转换成LodGroup
     * @return
     */
    ISceneLodGroup* AsSceneLodGroup(){return(this);}

    /**
     * @brief设置模型切换类型
     * @param emType
     */
    virtual void SetNodeChangeType(SCENE_NODE_CHANGE_TYPE emType)JUDGE_EQUAL_CALL_FUNCTION(emType,m_emType,ChangeTypeChanged)
    SCENE_NODE_CHANGE_TYPE NodeChangeType() const {return(m_emType);}

    /**
     * @brief 设置各级别的切换信息
     * @param rLevelInfos
     */
    virtual void SetLevelsInfo(const std::vector<float>& rLevelInfos)JUDGE_EQUAL_CALL_FUNCTION(rLevelInfos,m_vLevelInfos,LevelsChanged)
    virtual const std::vector<float>& LevelsInfo() const{return(m_vLevelInfos);}
protected:
    virtual ~ISceneLodGroup(){}

    virtual void ChangeTypeChanged()=0;
    virtual void LevelsChanged()=0;

protected:
    SCENE_NODE_CHANGE_TYPE m_emType{NODE_EYE_DISTANCE};
    std::vector<float>     m_vLevelInfos;
};

#endif//INTERFACE_SCENE_AUTO_SCALE_GROUP_HEARDER_H
