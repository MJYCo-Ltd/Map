#ifndef CMAP_MODEL_LAYER_H
#define CMAP_MODEL_LAYER_H

#include <osgEarth/VisibleLayer>

namespace osgEarth {
class AnnotationNode;
}

/**
 * @brief 由于AnnotationLayer使用起来有问题
 * 此处自己将AnnotationLayer的代码复制到此处
 */
class CMapModelLayer:public osgEarth::VisibleLayer
{
public:
    /**
     * @brief 默认构造函数
     */
    CMapModelLayer();

    /**
     * @brief 添加一个AnnotationNode
     */
    void addChild(osgEarth::AnnotationNode*);

    /**
     * @brief 获取Layer的根节点
     * @return
     */
    osg::Group* getGroup() const;

public:/// 重写VisibleLayer的接口

    virtual osg::Node* getNode() const;

    virtual void init();

protected:
    /**
     * @brief 析构函数
     */
    virtual ~CMapModelLayer(){}

private:

    osg::ref_ptr<osg::Group> _root; /// 图层的根节点
    void deserialize();
};

#endif // CMAP_MODEL_LAYER_H
