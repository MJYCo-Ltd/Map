#ifndef CMAP_MODEL_LAYER_H
#define CMAP_MODEL_LAYER_H

#include <osgEarth/VisibleLayer>
#include <Inner/Common/ImplSceneGroup.hpp>

/**
 * @brief 由于AnnotationLayer使用起来有问题
 * 此处自己将AnnotationLayer的代码复制到此处
 */
class CMapModelLayer:public osgEarth::VisibleLayer,public ImplSceneGroup<ISceneGroup>
{
public:
    /**
     * @brief 默认构造函数
     */
    CMapModelLayer(ISceneGraph* pSceneGraph);

    /**
     * @brief 获取Layer的根节点
     * @return
     */
    osg::Group* getGroup() const;

public:/// 重写VisibleLayer的接口

    virtual osg::Node* getNode() const;

    /**
     * @brief 重写初始化
     */
    virtual void init();

protected:
    void InitNode();
protected:
    /**
     * @brief 析构函数
     */
    virtual ~CMapModelLayer(){}
};

#endif // CMAP_MODEL_LAYER_H
