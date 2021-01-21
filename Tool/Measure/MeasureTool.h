#ifndef CMEASURETOOL_H
#define CMEASURETOOL_H

#include <string>
#include <NoQt.h>
#include <Tool/ITool.h>
class CMeasureTool
{
public:
    CMeasureTool();
};

extern "C"
{
    Q_DECL_EXPORT ITool* CreateTool(ISceneGraph* pSceneGraph,const std::string& sInterfaceName);
    Q_DECL_EXPORT bool QueryInterface(std::string& sInterfaceName);
};
#endif // CMEASURETOOL_H
