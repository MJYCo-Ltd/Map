#ifndef CMAPMODIFYLAYER_H
#define CMAPMODIFYLAYER_H

#include <osg/OperationThread>
#include <osgEarth/MapNode>

/**
 * @brief 地图图层变化
 */
class CMapModifyLayer : public osg::Operation
{
public:
    CMapModifyLayer(osgEarth::MapNode*,osgEarth::Layer*,bool);
    void operator()(osg::Object*);
private:
    osg::ref_ptr<osgEarth::MapNode> m_pMapNode;
    osg::ref_ptr<osgEarth::Layer>   m_pLayer;
    bool                            m_bAdd;
};

#endif // CMAPMODIFYLAYER_H
