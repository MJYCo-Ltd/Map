#ifndef INTERFACE_SCENE_LABEL_HEARDER_H
#define INTERFACE_SCENE_LABEL_HEARDER_H
#include <string>
#include <SceneGraph/ISceneNode.h>

/**
 * @brief 绘制模型的形状节点
 */
class ILabel:public ISceneNode
{
public:
    CONSTRUCTOR(ILabel,ISceneNode)

    /**
     * @brief 设置显示内容
     * @attention 编码方式为utf-8编码
     */
    void SetText(const std::string& sInfo)JUDGE_EQUAL_CALL_FUNCTION(sInfo,m_sInfo,InfoChanged)
    const std::string& Text()const {return(m_sInfo);}

    /**
     * @brief 设置显示内容
     * @attention 编码方式为utf-8编码
     */
    void SetFont(const std::string& sFont)JUDGE_EQUAL_CALL_FUNCTION(sFont,m_sFont,FontChanged)
    const std::string& Font()const{return(m_sFont);}

    /**
     * @brief 设置点的位置
     */
    void SetPos(const ScenePos& rPos)JUDGE_EQUAL_CALL_FUNCTION(rPos,m_stPos,PosChanged)
    const ScenePos& Pos()const{return(m_stPos);}

    /**
     * @brief 设置字体边框
     */
    void SetShowBoundingBox(bool bShow)JUDGE_EQUAL_CALL_FUNCTION(bShow,m_bShowBoundingBox,BoundingBoxVisibleChanged)
    bool ShowBoundingBox(){return(m_bShowBoundingBox);}

    /**
     * @brief 字体大小修改
     */
    void SetFontSize(float fFontSize)JUDGE_FLOAT_CALL_FUNCTION(fFontSize,m_fFontSize,FontSizeChanged)
    float FontSize()const{return(m_fFontSize);}

    /**
     * @brief 设置字体颜色
     */
    void SetFontFillColor(const SceneColor& rColor)JUDGE_EQUAL_CALL_FUNCTION(rColor,m_stFillColor,FillColorChanged)
    const SceneColor& FontFillColor() const{return(m_stFillColor);}

    /**
     * @brief 设置字体边框颜色
     */
    void SetFontOutColor(const SceneColor& rColor)JUDGE_EQUAL_CALL_FUNCTION(rColor,m_stOutColor,OutColorChanged)
    const SceneColor& FontOutColor() const{return(m_stOutColor);}

    /**
     * @brief 设置背景盒子颜色
     */
    void SetBoundingBoxColor(const SceneColor& rColor)JUDGE_EQUAL_CALL_FUNCTION(rColor,m_stBoundingBoxColor,BoundingBoxColorChanged)
    const SceneColor& BoundingBoxColor()const{return (m_stBoundingBoxColor);}

    /**
     * @brief 设置依附的节点
     */
    virtual bool SetAttachNode(ISceneNode* pSceneNode)=0;
    virtual bool DisAttachNode(ISceneNode* pSceneNode)=0;
    ISceneNode* AttachNode()const{return(m_pAttachNode);}
protected:
    virtual ~ILabel(){}
    virtual void FontChanged()=0;
    virtual void InfoChanged()=0;
    virtual void PosChanged()=0;
    virtual void BoundingBoxVisibleChanged()=0;
    virtual void FontSizeChanged()=0;
    virtual void FillColorChanged()=0;
    virtual void OutColorChanged()=0;
    virtual void BoundingBoxColorChanged()=0;

protected:
    ISceneNode* m_pAttachNode=nullptr;
    bool       m_bShowBoundingBox=false;
    float      m_fFontSize=20.f;
    std::string m_sInfo;
    std::string m_sFont;
    ScenePos   m_stPos;
    SceneColor m_stFillColor;
    SceneColor m_stOutColor;
    SceneColor m_stBoundingBoxColor;
};

#endif//INTERFACE_LABEL_HEARDER_H
