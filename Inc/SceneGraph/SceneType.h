#ifndef H_SCENE_TYPE_INCLUDE_H
#define H_SCENE_TYPE_INCLUDE_H

#include <string>
#include <cmath>
#include <cfloat>

#define JUDGE_DOUBLE_CALL_FUNCTION(VA,VB,VC) {if(fabs(VA-VB)>DBL_EPSILON){VB=VA;VC();}}
#define JUDGE_FLOAT_CALL_FUNCTION(VA,VB,VC) {if(fabs(VA-VB)>FLT_EPSILON){VB=VA;VC();}}

#define JUDGE_POSITIVE_DOUBLE_CALL_FUNCTION(VA,VB,VC) {if(VA>0.&&fabs(VA-VB)>DBL_EPSILON){VB=VA;VC();}}
#define JUDGE_POSITIVE_FLOAT_CALL_FUNCTION(VA,VB,VC) {if(VA>0.f&&fabs(VA-VB)>FLT_EPSILON){VB=VA;VC();}}

#define JUDGE_EQUAL_CALL_FUNCTION(VA,VB,VC) {if(VA != VB){VB = VA;VC();}}
#define JUDGE_EQUAL_SET_TRUE(VA,VB,VC) {if(VA != VB){VB = VA;VC=true;}}

#define CONSTRUCTOR(VA,VB) VA(ISceneGraph* pSceneGraph):VB(pSceneGraph){}
#define SET_TRUE_NODE_UPDATE(VA) {VA=true;IOsgSceneNode::NodeChanged();}
#define SET_TRUE_SHAPE_UPDATE(VA) {VA=true;ShapeChanged();}

/**
 * @brief 俯仰翻滚偏航的旋转类型
 */
enum Scene_ROTATE
{
    SR_XYZ,SR_XZY,SR_YXZ,SR_YZX,SR_ZXY,SR_ZYX /// 上下两种方式相同
   ,SR_RPY,SR_RYP,SR_PRY,SR_PYR,SR_YRP,SR_YPR
};

/// 场景颜色
struct SceneColor
{
    float fR=1.f;
    float fG=1.f;
    float fB=1.f;
    float fA=1.f;

    bool operator == (const SceneColor& rOther) const
    {
        if(&rOther == this)
        {
            return(true);
        }

        return(fabs(fR - rOther.fR) < FLT_EPSILON
         &&fabs(fG - rOther.fG) < FLT_EPSILON
         &&fabs(fB - rOther.fB) < FLT_EPSILON
         &&fabs(fA - rOther.fA) < FLT_EPSILON);
    }

    bool operator !=(const SceneColor& rOther) const
    {
        return(!(this->operator==(rOther)));
    }
};

/// 姿态信息
struct SceneAttitude
{
    Scene_ROTATE rotaOrder=SR_RYP; /// 旋转顺序
    double dYaw=0.;    /// 方位 [0~360)  绕Z轴旋转
    double dPitch=0.;  /// 俯仰 [-90,90] 绕Y轴旋转
    double dRoll=0.;   /// 翻滚 [0~360)  绕X轴旋转

    bool operator == (const SceneAttitude& rOther) const
    {
        if(&rOther == this)
        {
            return(true);
        }

        return(rOther.rotaOrder == rotaOrder
         && fabs(dYaw - rOther.dYaw) < DBL_EPSILON
         && fabs(dPitch - rOther.dPitch) < DBL_EPSILON
         && fabs(dRoll - rOther.dRoll) < DBL_EPSILON);
    }

    bool operator !=(const SceneAttitude& rOther) const
    {
        return(!(this->operator==(rOther)));
    }
};

/**
 * @brief 位置结构体
 */
struct ScenePos
{
    float fX=0.f;    /// x轴坐标 或者 经度[deg]
    float fY=0.f;    /// y轴坐标 或者 纬度[deg]
    float fZ=0.f;    /// z轴坐标 或者 高度[m]

    bool operator == (const ScenePos& rOther) const
    {
        if(&rOther == this)
        {
            return(true);
        }

        return(fabs(fX - rOther.fX) < FLT_EPSILON
        &&fabs(fY - rOther.fY) < FLT_EPSILON
        &&fabs(fZ - rOther.fZ) < FLT_EPSILON);
    }

    bool operator !=(const ScenePos& rOther) const
    {
        return(!(this->operator==(rOther)));
    }
};

/**
 * @brief 屏幕偏移
 */
struct ScenePixelOffset
{
    short sWidth=0;
    short sHeight=0;

    bool operator ==(const ScenePixelOffset& rOther) const
    {
        if(&rOther == this)
        {
            return(true);
        }

        return(rOther.sHeight == sHeight
            && rOther.sWidth == sWidth);
    }

    bool operator !=(const ScenePixelOffset& rOther) const
    {
        return(!(this->operator==(rOther)));
    }
};

/**
 * @brief 内存数据结构体
 * @attention 只支持rgba每一个颜色8bit(1byte)的数据结构
 * @例如 10X10像素的图片 大小为 10x10x4=400byte的数据
 */
struct RGBAData
{

    unsigned short unWidth;
    unsigned short unHeight;
    unsigned char* pRGBAData;
    bool           bFlipVertically;

    RGBAData()
    {
        unWidth=0u;
        unHeight=0u;
        pRGBAData=nullptr;
        bFlipVertically=false;
    }

    RGBAData(const RGBAData& rOther)
    {
        unWidth = rOther.unWidth;
        unHeight = rOther.unHeight;
        bFlipVertically = rOther.bFlipVertically;

        int nSize = unWidth*unHeight*4;
        pRGBAData = new unsigned char[nSize]();
        memcpy(pRGBAData,rOther.pRGBAData,nSize);
    }

    RGBAData& operator=(const RGBAData& rOther)
    {
        if(&rOther == this)
        {
            return(*this);
        }
        else
        {
            unWidth = rOther.unWidth;
            unHeight = rOther.unHeight;
            bFlipVertically = rOther.bFlipVertically;

            int nSize = unWidth*unHeight*4;
            pRGBAData = new unsigned char[nSize]();
            memcpy(pRGBAData,rOther.pRGBAData,nSize);
        }
    }
    ~RGBAData()
    {
        delete [] pRGBAData;
    }

    bool operator == (const RGBAData& rOther)const
    {
        if(this == & rOther)
        {
            return (true);
        }

        return(unWidth == rOther.unWidth
               && unHeight == rOther.unHeight
               && pRGBAData == rOther.pRGBAData
               && bFlipVertically == rOther.bFlipVertically);
    }

    bool operator !=(const RGBAData& rOther)const
    {
        return(!this->operator==(rOther));
    }
};
static unsigned int const PICK_MASK=1u;
#endif
