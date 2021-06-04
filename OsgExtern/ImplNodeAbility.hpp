#ifndef IMPLNODEABILITY_HPP
#define IMPLNODEABILITY_HPP

#include <SceneGraph/ISceneNode.h>
#include <Inner/IOsgSceneNode.h>

#define SET_TRUE_ABILITY_UPDATE(VA) {VA=true;m_pSceneNode->AbilityChanged();}

template <typename T>
class ImplNodeAbility:public T
{
public:
    virtual void InitAbility()=0;
    /**
     * @brief 绑定场景节点
     * @param pNode 场景节点
     */
    void BoundNode(ISceneNode* pNode)
    {
        if(nullptr != pNode && nullptr == m_pSceneNode)
        {
            m_pSceneNode = pNode;
            m_bNeedInit=true;
            m_pSceneNode->AbilityChanged();
        }
    }

    /**
     * @brief 能力状态更改
     */
    void AbilityStatusChanged()
    {
        m_pSceneNode->AbilityChanged();
    }

    /**
     * @brief 更新节点的能力
     */
    void UpdateAbility()
    {
        if(m_bNeedInit)
        {
            InitAbility();
            m_bNeedInit = false;
        }
    }
protected:
    ISceneNode* m_pSceneNode{};
    bool        m_bNeedInit{false};
};

#endif // IMPLNODEABILITY_HPP
