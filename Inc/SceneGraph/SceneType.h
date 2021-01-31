#ifndef H_SCENE_TYPE_INCLUDE_H
#define H_SCENE_TYPE_INCLUDE_H

#include <cmath>
#include <cfloat>

#define JUDGE_DOUBLE_CALL_FUNCTION(VA,VB,VC) {if(fabs(VA-VB)>DBL_EPSILON){VB=VA;VC();}}
#define JUDGE_FLOAT_CALL_FUNCTION(VA,VB,VC) {if(fabs(VA-VB)>FLT_EPSILON){VB=VA;VC();}}

#define JUDGE_DOUBLE_SET_TRUE(VA,VB,VC) {if(fabs(VA-VB)>DBL_EPSILON){VB=VA;VC=true;}}
#define JUDGE_FLOAT_SET_TRUE(VA,VB,VC) {if(fabs(VA-VB)>FLT_EPSILON){VB=VA;VC=true;}}

#define JUDGE_EQUAL_CALL_FUNCTION(VA,VB,VC) {if(VA != VB){VB = VA;VC();}}
#define JUDGE_EQUAL_SET_TRUE(VA,VB,VC) {if(VA != VB){VB = VA;VC=true;}}

#define CONSTRUCTOR(VA,VB) VA(ISceneGraph* pSceneGraph):VB(pSceneGraph){}
#define SET_TRUE_NODE_UPDATE(VA) {VA=true;NodeChanged();}

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
    float fX=0.f;    /// x轴坐标
    float fY=0.f;    /// y轴坐标
    float fZ=0.f;    /// z轴坐标

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

static unsigned int const PICK_MASK=1u;
#endif
