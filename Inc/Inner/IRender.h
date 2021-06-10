#ifndef INCLUDE_HEADER_SCENERENDER_H
#define INCLUDE_HEADER_SCENERENDER_H

#include <osg/OperationThread>
#include <osgViewer/View>

/**
 * @brief 仿真运行控制器
 */
class IRender
{
public:

    /**
     * @brief 添加更新操作
     * @param pUpdate
     */
    virtual void AddUpdateOperation(osg::Operation*)=0;

    /**
     * @brief 重建线程
     */
    virtual void ResetupThread()=0;
protected:
    virtual ~IRender(){}
};

#endif
