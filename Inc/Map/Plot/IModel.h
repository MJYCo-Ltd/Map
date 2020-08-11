#ifndef INTERFACE_MODEL_HEARDER_H
#define INTERFACE_MODEL_HEARDER_H
#include <string>
#include "IDrawShape.h"
using namespace std;

enum PIC_MODEL_CHANGE_TYPE
{
    EYE_DISTANCE, ///距离视点的距离
    MODEL_PIXEL   ///像素大小
};

/**
 * @brief 模型节点
 */
class IModel:public IDrawShape
{
public:
    /**
     * @brief 设置二维和模型变小时显示的图片
     */
    virtual void Set2DAndSamllPic(const string&)=0;

    /**
     * @brief 设置模型路径
     */
    virtual void SetModelPath(const string&)=0;

    /**
     * @brief 设置图片和模型切换的方式
     * @attention 当切换方式为 EYE_DISTANCE 第二个参数为距离值,单位为m
     * @attention 当切换方式为 MODEL_PIXEL  第二个参数为最大方向上的像素大小
     */
    virtual void SetPicModelChange(PIC_MODEL_CHANGE_TYPE,double)=0;

    /**
     * @brief 是否开启自动缩放
     */
    virtual void SetAutoScal(bool)=0;

    /**
     * @brief 设置缩放系数
     */
    virtual void SetScalBit(double)=0;

    /**
     * @brief 设置姿态
     */
    virtual void SetYPR(double,double,double)=0;
};

#endif
