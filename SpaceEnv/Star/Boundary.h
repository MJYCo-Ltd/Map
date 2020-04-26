#ifndef BOUNDARY_H
#define BOUNDARY_H

#include <osg/Geometry>
class CBoundary:public osg::Referenced
{
public:
    CBoundary();

    /**
     * @brief 设置是否可见
     * @param bVisible
     */
    void SetVisible(bool bVisible);

    osg::Node* getNode();
protected:

    ~CBoundary();
private:
    void makeBoundray();

private:
    osg::ref_ptr<osg::Geometry> m_pBoundray;
    bool                        m_bShowBoundary; /// 是否显示天区
};

#endif // BOUNDARY_H
