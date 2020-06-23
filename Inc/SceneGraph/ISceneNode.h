#ifndef INTERFACE_SCENE_NODE_HEARDER_H
#define INTERFACE_SCENE_NODE_HEARDER_H

#include <cmath>
/**
 * @brief 视点位置
 */
union ScenePos
{
    struct
    {
        float fLon;    /// 经度 [度]
        float fLat;    /// 纬度 [度]
        float fHeight; /// 高度 [米]
        bool  bIsGeo;  /// 是否是经纬度
    };

    struct
    {
        float fX;    /// x轴坐标
        float fY;    /// y轴坐标
        float fZ;    /// z轴坐标
    };

    bool operator == (const ScenePos& rOther) const
    {
        if(&rOther == this)
        {
            return(true);
        }

        if(fabs(fLon - rOther.fLon) < 1e-6
         &&fabs(fLat - rOther.fLat) < 1e-6
         &&fabs(fHeight - rOther.fHeight) < 1e-6
         && bIsGeo == rOther.bIsGeo)
        {
            return(true);
        }
        else
        {
            return(false);
        }
    }

    bool operator !=(const ScenePos& rOther) const
    {
        return(!(this->operator==(rOther)));
    }
};

class ISceneGraph;
/**
 * @brief 场景节点类
 */
class ISceneNode
{
public:
    virtual ~ISceneNode(){}

    /**
     * @brief 设置场景节点的位置
     */
    virtual void SetPos(const ScenePos&)=0;

    /**
     * @brief 获取当前节点位置
     * @return
     */
    virtual const ScenePos& GetPos()=0;

    /**
     * @brief 添加场景节点
     */
    virtual bool AddSceneNode(ISceneNode*)=0;

    /**
     * @brief 删除场景节点
     * @return
     */
    virtual bool RemoveSceneNode(ISceneNode*)=0;

    /**
     * @brief 设置节点可见
     */
    virtual void SetVisible(bool)=0;

    /**
     * @brief 判断节点是否可见
     * @return
     */
    virtual bool IsVisible()=0;


    /**
     * @brief 获取绑定的场景
     * @return
     */
    virtual ISceneGraph* GetBindSceneGraph(){return(m_pSceneGraph);}
protected:
    ISceneGraph* m_pSceneGraph;
};

#endif
