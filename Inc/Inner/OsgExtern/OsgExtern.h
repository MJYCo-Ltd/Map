#ifndef OSGEXTERN_HEADER_H
#define OSGEXTERN_HEADER_H
#include <osg/OperationThread>
#include <osg/Group>

#include <Inner/OsgExtern/OsgExtern_Global.h>

/**
 * @brief 将显示节点从场景中移除
 */
class OSGEXTERNSHARED_EXPORT RemoveFromeScene:public osg::Operation
{
public:
    RemoveFromeScene(osg::Node* pParent);

    /**
     * @brief 回调
     */
    void operator()(osg::Object*);
private:
    osg::ref_ptr<osg::Node> m_pParent;
};

/**
 * @brief 清空所有子节点
 */
class OSGEXTERNSHARED_EXPORT CClearChildNode:public osg::Operation
{
public:
    CClearChildNode(osg::Group* pParent);
    ~CClearChildNode();
    void operator()(osg::Object*);
private:
    osg::ref_ptr<osg::Group> m_pParent; /// 父节点
};

/**
 * @brief 修改节点数据
 */
class OSGEXTERNSHARED_EXPORT CModifyNode:public osg::Operation
{
public:
    CModifyNode(osg::Group* pParent,
                osg::Node* pChild,
                bool bAdd);
    ~CModifyNode();

    void operator()(osg::Object*);
private:
    osg::ref_ptr<osg::Group> m_pParent; /// 父节点
    osg::ref_ptr<osg::Node>  m_pChild;  /// 子节点
    bool                     m_bAdd;    /// 是否增加
};

#endif // MAP_GLOBAL_H
