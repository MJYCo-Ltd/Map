#ifndef CTOOLSELECTOR_H
#define CTOOLSELECTOR_H

#include <NoQt.h>
#include <Tools/ITool.h>
class ISceneGraph;

class CToolSelector:public ITool
{
public:
    CONSTRUCTOR(CToolSelector,ITool)

    void RegisterTool();
};

extern "C"
{
    Q_DECL_EXPORT ITool* CreateToolSelector(ISceneGraph* pSceneGraph);
};
#endif // CTOOLSELECTOR_H
