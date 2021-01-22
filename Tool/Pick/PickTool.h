#ifndef CPICK_TOOL_H
#define CPICK_TOOL_H

#include <string>
#include <NoQt.h>
#include "../IToolBase.h"

extern "C"
{
    Q_DECL_EXPORT IToolBase *CreateTool(ISceneGraph* pSceneGraph,
                                        CToolSelector* pToolSelector,
                                        const std::string& sInterfaceName);
    Q_DECL_EXPORT bool QueryInterface(std::string& sInterfaceName);
};
#endif // CPICK_TOOL_H
