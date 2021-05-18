#ifndef INTERFACE_SCENE_NODE_HEARDER_H
#define INTERFACE_SCENE_NODE_HEARDER_H

#include <SceneGraph/SceneType.h>

class ISceneGraph;
class IHudNode;
class ISceneGroup;
class ISceneLodGroup;
class ISceneFlashGroup;
class ISceneScaleGroup;
class ISceneCameraGroup;
class ISceneAttitudeGroup;
class ISceneScreenGroup;
class ISceneVisualGroup;

class ISceneModel;
class IOsgSceneNode;

class IMapSceneNode;

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
     * @brief 设置节点是否可以被点选
     */
    void SetCanPick(bool bCanPick)JUDGE_EQUAL_CALL_FUNCTION(bCanPick,m_bCanPick,PickStateChanged)
    bool CanPick(){return(m_bCanPick);}
    unsigned int PickID(){return(m_unID);}

    /**
     * @brief 设置节点是否开启光照
     */
    void OpenLight(bool bOpen)JUDGE_EQUAL_CALL_FUNCTION(bOpen,m_bOpenLight,LightChanged)
    bool IsOpenLight() const{return(m_bOpenLight);}

    /**
     * @brief 设置节点总是显示
     */
    void AlwasOnTop(bool bOpen)JUDGE_EQUAL_CALL_FUNCTION(bOpen,m_bShowTop,ShowTopChanged)
    bool IsAlwasOnTop()const{return(m_bShowTop);}

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
    virtual ISceneCameraGroup* AsSceneCameraGroup(){return(nullptr);}
    virtual ISceneScreenGroup* AsSceneScreenGroup(){return(nullptr);}
    virtual ISceneVisualGroup* AsSceneVisualGroup(){return(nullptr);}
    virtual IMapSceneNode* AsMapSceneNode(){return(nullptr);}
    virtual IOsgSceneNode* AsOsgSceneNode(){return(nullptr);}
    virtual IHudNode* AsHudNode(){return(nullptr);}
protected:
    virtual ~ISceneNode(){}
    virtual void VisibleChanged()=0;
    virtual void PickStateChanged()=0;
    virtual void LightChanged()=0;
    virtual void ShowTopChanged()=0;

protected:
    ISceneGraph* m_pSceneGraph;
    bool         m_bVisible=true;
    bool         m_bCanPick=false;
    bool         m_bOpenLight=false;
    bool         m_bShowTop=false;
    unsigned int m_unID=0;
};

#endif
