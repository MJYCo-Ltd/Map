#ifndef CMAPNODECHANGED_H
#define CMAPNODECHANGED_H
#include <osg/OperationThread>
#include <osgEarth/MapNode>
class CMap;

class CMapNodeChanged : public osg::Operation
{
public:
    CMapNodeChanged(osgEarth::MapNode*,osgEarth::MapNode*,CMap*);
    void operator()(osg::Object*);
private:
    osg::ref_ptr<osgEarth::MapNode> m_pOldMapNode;
    osg::ref_ptr<osgEarth::MapNode> m_pNewMapNode;
    CMap*                           m_pMap;
};

#endif // CMAPNODECHANGED_H
