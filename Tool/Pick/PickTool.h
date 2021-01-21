#ifndef CPICK_TOOL_H
#define CPICK_TOOL_H

#include <string>
#include <NoQt.h>
#include <Tool/IToolBase.h>
class CPickTool
{
public:
    CPickTool();
};

extern "C"
{
    Q_DECL_EXPORT IToolBase *CreateTool(ISceneGraph* pSceneGraph,const std::string& sInterfaceName);
    Q_DECL_EXPORT bool QueryInterface(std::string& sInterfaceName);
};
#endif // CPICK_TOOL_H
