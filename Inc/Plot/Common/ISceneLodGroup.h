#ifndef INTERFACE_SCENE_LOD_GROUP_HEARDER_H
#define INTERFACE_SCENE_LOD_GROUP_HEARDER_H
#include <vector>
#include <SceneGraph/ISceneGroup.h>


enum SCENE_NODE_CHANGE_TYPE
{
    NODE_EYE_DISTANCE, ///距离视点的距离
    NODE_SCREEN_PIXEL  ///模型在屏幕上的像素大小(此方法对自动缩放的模型无效)
};

/**
 * @brief 各级别的信息
 */
struct LevelInfo
{
    int  nLevle;  ///要设置的级别
    float fInfo;  ///与下一级进行切换的数据

    bool operator ==(const LevelInfo& rOther) const
    {
        if(&rOther == this)
        {
            return(true);
        }

        return(rOther.nLevle == nLevle
         &&fabs(rOther.fInfo-fInfo) < FLT_EPSILON);
    }

    bool operator!=(const LevelInfo& rOther) const
    {
        return(!this->operator==(rOther));
    }
};

typedef std::vector<LevelInfo> LevelInfos;

/**
 * @brief 场景切换节点，根据需要切换不同的显示
 */
class ISceneLodGroup:public ISceneGroup
{
public:
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
     * @brief 设置可以切换的级别数
     * @param nLevels
     */
    virtual void SetNumberLevels(int nLevels){if(nLevels>0 && nLevels != m_nLevels){m_nLevels=nLevels;LevelsChanged();}}
    int NumberLevels() const {return(m_nLevels);}

    /**
     * @brief 设置各级别的切换信息
     * @param rLevelInfos
     */
    virtual void SetLevelsInfo(const LevelInfos& rLevelInfos)JUDGE_EQUAL_CALL_FUNCTION(rLevelInfos,m_vLevlInfos,LevelsChanged)
    virtual const LevelInfos& LevelsInfo() const{return(m_vLevlInfos);}
protected:
    virtual ~ISceneLodGroup(){}

    virtual void ChangeTypeChanged()=0;
    virtual void LevelsChanged()=0;

protected:
    SCENE_NODE_CHANGE_TYPE m_emType=NODE_EYE_DISTANCE;
    int                    m_nLevels=2;
    LevelInfos             m_vLevlInfos;
};

#endif//INTERFACE_SCENE_AUTO_SCALE_GROUP_HEARDER_H
