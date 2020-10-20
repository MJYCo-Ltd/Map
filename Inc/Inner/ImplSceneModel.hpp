#ifndef IMPL_SCENE_MODEL_H
#define IMPL_SCENE_MODEL_H
#include <Inner/OsgExtern/MyMatrixTransform.h>
#include "ImplSceneNode.hpp"

/**
 *  实现ISceneModel的所有的接口
 */
template <typename T>
class ImplSceneModel:public ImplSceneNode<T>
{
public:
    CONSTRUCTOR(ImplSceneModel,ImplSceneNode<T>)

    /**
     * @brief设置osg节点
     */
    void SetModel(osg::Node* pNode)
    {
        m_pModel = pNode;
        m_pInitAttitudeMatrix->addChild(m_pModel);
    }

    /**
     * @brief 是否可以做为模型节点
     * @return
     */
    ISceneModel* AsSceneModel(){return(this);}
protected:
    /**
     * @brief 初始化节点
     */
    void InitNode()
    {
        m_pInitAttitudeMatrix = new CMyMatrixTransform;
        SetOsgNode(m_pInitAttitudeMatrix);
    }

    /**
     * @brief 显隐状态更改
     */
    void InitAttitudeChanged()
    {
    }

protected:
    osg::ref_ptr<osg::Node>          m_pModel;
    CMyMatrixTransform* m_pInitAttitudeMatrix;
};

#endif // IMPL_SCENE_MODEL_H
