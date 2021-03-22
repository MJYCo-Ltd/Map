#ifndef INTERFACE_TOOL_PUB_HEADER_H
#define INTERFACE_TOOL_PUB_HEADER_H

/**
 * @brief 工具发送消息
 */
class IToolPub
{
public:
    virtual ~IToolPub(){}
    /**
     * @brief 释放工具
     */
    virtual void PickID(unsigned int,unsigned int)=0;
};

#endif//INTERFACE_TOOL_HEADER_H
