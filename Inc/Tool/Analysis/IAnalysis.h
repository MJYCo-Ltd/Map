#ifndef INTERFACE_ANALYSIS_HEADER_H
#define INTERFACE_ANALYSIS_HEADER_H

#include <Tool/ITool.h>

/**
 * @brief 分析类基类
 */
class IAnalysis：public ITool
{
public:
    CONSTRUCTOR(IAnalysis,ITool)

protected:
    virtual ~IAnalysis(){}
};

#endif//INTERFACE_TOOL_HEADER_H
