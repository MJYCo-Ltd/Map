#ifndef H_HEADER_CSCENELINE_H
#define H_HEADER_CSCENELINE_H

#include <osgEarth/FeatureNode>
#include <Plot/Map/IMapLine.h>
#include <Inner/ImplMapSceneGeometry.hpp>

class CMapLine:public ImplMapSceneGeometry<IMapLine>
{
public:
    CONSTRUCTOR(CMapLine,ImplMapSceneGeometry<IMapLine>)

    virtual bool AddPoint(int,const ScenePos&) override;
    virtual bool UpdatePoint(int,const ScenePos&) override;
    virtual bool DelPoint(int,const ScenePos&) override;
    virtual void SetMultPos(const std::list<ScenePos>&)override;
    /**
     * @brief 初始化节点
     */
    void InitNode() override;
protected:
    void PointChange();
    std::list<ScenePos>    m_listAllPos;
    std::vector<ScenePos>    m_listVirtualPos; //细分计算虚拟节点
};

#endif // CSCENELINE_H
