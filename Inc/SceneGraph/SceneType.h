#ifndef H_SCENE_TYPE_INCLUDE_H
#define H_SCENE_TYPE_INCLUDE_H

#include <cstring>
#include <string>
#include <cmath>
#include <cfloat>

/// 判断浮点数相等的宏
#define JUDGE_DOUBLE_EQUAL(VA,VB) (fabs(VA-VB)<DBL_EPSILON)
#define JUDGE_FLOAT_EQUAL(VA,VB)  (fabs(VA-VB)<FLT_EPSILON)

/// 判断浮点数不相等的宏
#define JUDGE_DOUBLE_DONT_EQUAL(VA,VB) (fabs(VA-VB)>DBL_EPSILON)
#define JUDGE_FLOAT_DONT_EQUAL(VA,VB)  (fabs(VA-VB)>FLT_EPSILON)

/// 判断浮点数不相等调用函数的宏
#define JUDGE_DOUBLE_CALL_FUNCTION(VA,VB,VC) {if JUDGE_DOUBLE_DONT_EQUAL(VA,VB){VB=VA;VC();}}
#define JUDGE_FLOAT_CALL_FUNCTION(VA,VB,VC) {if JUDGE_FLOAT_DONT_EQUAL(VA,VB){VB=VA;VC();}}

/// 判断浮点数为正且不相等调用函数的宏
#define JUDGE_POSITIVE_DOUBLE_CALL_FUNCTION(VA,VB,VC) {if(VA>0. && JUDGE_DOUBLE_DONT_EQUAL(VA,VB)){VB=VA;VC();}}
#define JUDGE_POSITIVE_FLOAT_CALL_FUNCTION(VA,VB,VC) {if(VA>0.f && JUDGE_FLOAT_DONT_EQUAL(VA,VB)){VB=VA;VC();}}

#define JUDGE_EQUAL_CALL_FUNCTION(VA,VB,VC) {if(VA != VB){VB = VA;VC();}}
#define JUDGE_EQUAL_SET_TRUE(VA,VB,VC) {if(VA != VB){VB = VA;VC=true;}}

#define CONSTRUCTOR(VA,VB) VA(ISceneGraph* pSceneGraph):VB(pSceneGraph){}
#define SET_TRUE_NODE_UPDATE(VA) {VA=true;this->NodeChanged();}
#define SET_TRUE_SHAPE_UPDATE(VA) {VA=true;this->ShapeChanged();}

enum ABILITY_TYPE
{
    NO_ABILITY=0,               /// 没有能力
    FLASH_ABILITY=0x1,          /// 闪烁
    AUTOSCAL_ABILITY=0x2,       /// 自动缩放
    MOTION_ABILITY=0x4          /// 运动能力
};

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
    SceneColor(float r = 1.f,float g = 1.f,float b = 1.f,float a = 1.f):fR(r),fG(g),fB(b),fA(a)
    {
    }

    float fR;
    float fG;
    float fB;
    float fA;

    bool operator == (const SceneColor& rOther) const
    {
        if(&rOther == this)
        {
            return(true);
        }

        return(JUDGE_FLOAT_EQUAL(fR,rOther.fR)
         && JUDGE_FLOAT_EQUAL(fG,rOther.fG)
         && JUDGE_FLOAT_EQUAL(fB,rOther.fB)
         && JUDGE_FLOAT_EQUAL(fA,rOther.fA));
    }

    bool operator !=(const SceneColor& rOther) const
    {
        return(!(this->operator==(rOther)));
    }
};

/// 姿态信息
struct SceneAttitude
{
    Scene_ROTATE rotaOrder{SR_RYP}; /// 旋转顺序
    double dYaw{};    /// 方位 [0~360)  绕Z轴旋转
    double dPitch{};  /// 俯仰 [-90,90] 绕Y轴旋转
    double dRoll{};   /// 翻滚 [0~360)  绕X轴旋转

    bool operator == (const SceneAttitude& rOther) const
    {
        if(&rOther == this)
        {
            return(true);
        }

        return(rOther.rotaOrder == rotaOrder
         && JUDGE_DOUBLE_EQUAL(dYaw,rOther.dYaw)
         && JUDGE_DOUBLE_EQUAL(dPitch,rOther.dPitch)
         && JUDGE_DOUBLE_EQUAL(dRoll,rOther.dRoll));
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
    double dX{};    /// x轴坐标 或者 经度[deg]
    double dY{};    /// y轴坐标 或者 纬度[deg]
    double dZ{};    /// z轴坐标 或者 高度[m]

    bool operator == (const ScenePos& rOther) const
    {
        if(&rOther == this)
        {
            return(true);
        }

        return(JUDGE_DOUBLE_EQUAL(dX,rOther.dX)
        &&JUDGE_DOUBLE_EQUAL(dY,rOther.dY)
        &&JUDGE_DOUBLE_EQUAL(dZ,rOther.dZ));
    }

    bool operator !=(const ScenePos& rOther) const
    {
        return(!(this->operator==(rOther)));
    }

    bool IsValidMap()
    {
        return(dX>-180&&dX<180&&dY>=-90&&dY<=-90);
    }
};

/**
 * @brief 屏幕偏移
 */
struct ScenePixelOffset
{
    short sWidth{};
    short sHeight{};

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

    unsigned short unWidth{};
    unsigned short unHeight{};
    unsigned int   unDataSize{};
    unsigned char* pRGBAData{};
    bool           bFlipVertically{false};
    std::string    sInnerName;

    RGBAData(){}

    RGBAData(const RGBAData& rOther)
    {
        unWidth = rOther.unWidth;
        unHeight = rOther.unHeight;
        bFlipVertically = rOther.bFlipVertically;

        int nSize = unWidth*unHeight*4;
        pRGBAData = new unsigned char[nSize]();
        memcpy(pRGBAData,rOther.pRGBAData,nSize);
        sInnerName = rOther.sInnerName;
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
            sInnerName = rOther.sInnerName;
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
               && bFlipVertically == rOther.bFlipVertically
               && sInnerName == rOther.sInnerName);
    }

    bool operator !=(const RGBAData& rOther)const
    {
        return(!this->operator==(rOther));
    }
};
static unsigned int const PICK_MASK{1u};
#endif
