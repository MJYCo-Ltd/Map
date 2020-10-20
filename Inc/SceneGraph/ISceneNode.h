#ifndef INTERFACE_SCENE_NODE_HEARDER_H
#define INTERFACE_SCENE_NODE_HEARDER_H

#include <SceneGraph/SceneType.h>

class ISceneGraph;
class ISceneGroup;
class ISceneModel;
class IOsgSceneNode;
class IMapSceneNode;
class ISceneLodGroup;
class ISceneFlashGroup;
class ISceneScaleGroup;
class ISceneAttitudeGroup;

#define JUDGE_DOUBLE_CALL_FUNCTION(VA,VB,VC) {if(fabs(VA-VB)>DBL_EPSILON){VB=VA;VC();}}
#define JUDGE_FLOAT_CALL_FUNCTION(VA,VB,VC) {if(fabs(VA-VB)>FLT_EPSILON){VB=VA;VC();}}

#define JUDGE_DOUBLE_SET_TRUE(VA,VB,VC) {if(fabs(VA-VB)>DBL_EPSILON){VB=VA;VC=true;}}
#define JUDGE_FLOAT_SET_TRUE(VA,VB,VC) {if(fabs(VA-VB)>FLT_EPSILON){VB=VA;VC=true;}}

#define JUDGE_EQUAL_CALL_FUNCTION(VA,VB,VC) {if(VA != VB){VB = VA;VC();}}
#define JUDGE_EQUAL_SET_TRUE(VA,VB,VC) {if(VA != VB){VB = VA;VC=true;}}

#define CONSTRUCTOR(VA,VB) VA(ISceneGraph* pSceneGraph):VB(pSceneGraph){}
/**
 * @brief 场景节点类
 */
class ISceneNode
{
public:
    CONSTRUCTOR(ISceneNode,m_pSceneGraph)

    /**
     * @brief 获取节点绑定的场景
     * @return
     */
    const ISceneGraph* GetBoundSceneGraph() const{return(m_pSceneGraph);}

    /**
     * @brief 设置节点可见
     */
    void SetVisible(bool bVisible) JUDGE_EQUAL_CALL_FUNCTION(bVisible,m_bVisible,VisibleChanged)
    bool IsVisible() const{return(m_bVisible);}

    /**
     * @brief 状态更改消息
     */
    virtual void VisibleChanged()
    {
        m_bVisibleChanged = true;
    }

    /**
     * @brief 转换成想要的类型
     * @return
     */
    virtual ISceneGroup* AsSceneGroup(){return(nullptr);}
    virtual ISceneModel* AsSceneModel(){return(nullptr);}
    virtual ISceneLodGroup* AsSceneLodGroup(){return(nullptr);}
    virtual ISceneScaleGroup* AsSceneScaleGroup(){return(nullptr);}
    virtual ISceneFlashGroup* AsSceneFlashGroup(){return(nullptr);}
    virtual ISceneAttitudeGroup* AsSceneAttitudeGroup(){return(nullptr);}
    virtual IMapSceneNode* AsMapSceneNode(){return(nullptr);}
    virtual IOsgSceneNode* AsOsgSceneNode(){return(nullptr);}
protected:
    virtual ~ISceneNode(){}

protected:
    ISceneGraph* m_pSceneGraph;
    bool         m_bVisible=true;
    bool         m_bVisibleChanged=false;
};

#endif
