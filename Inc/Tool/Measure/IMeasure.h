#ifndef INTERFACE_MEASURE_HEADER_H
#define INTERFACE_MEASURE_HEADER_H

#include <Tool/ITool.h>

/**
 * @brief 量算基类
 */
class IMeasure：public ITool
{
public:
    CONSTRUCTOR(IMeasure,ITool)

protected:
    virtual ~IMeasure(){}
};

#endif//INTERFACE_TOOL_HEADER_H
