#ifndef INCLUDE_HEADER_SCENERENDER_H
#define INCLUDE_HEADER_SCENERENDER_H

#include <osg/OperationThread>
#include <osgViewer/View>

/**
 * @brief 每次渲染调用的回调
 */
struct RenderCall
{
    bool m_bCallOne{true};
    /**
     * @brief 每帧调用一次
     */
    virtual void FrameCall(){}
};

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
     * @attention 只有当增加删除视口的才需要调用此方法
     */
    virtual void ResetupThread()=0;

    /**
     * @brief 订阅消息
     */
    virtual void SubMessage(RenderCall*)=0;

    /**
     * @brief 更新所有的渲染消息
     */
    virtual void UpdateRender()=0;
protected:
    virtual ~IRender(){}
};

#endif
