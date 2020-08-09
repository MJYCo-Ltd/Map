#ifndef H_HEADER_CSCENELINE_H
#define H_HEADER_CSCENELINE_H

#include <osgEarth/FeatureNode>
#include <Map/Plot/ILine.h>
#include <Inner/QtDrawShape.h>

enum LINE_CHANGE_TYPE
{
    NO_CHANGE,    /// 没有更改
    UPDATE_POINT, /// 更新点
    REPLACE_POINS /// 替换所有的点
};

class CSceneLine:public QtDrawShape<ILine>
{
    friend class LineCallBack;
public:
    CSceneLine(ISceneGraph*);


    /**
     * @brief 增加点
     */
    virtual void AddPoint(int,const ScenePos&);

    /**
     * @brief 移除点
     */
    virtual bool RemovePoint(int);

    /**
     * @brief 更新指定位置
     */
    virtual bool UpdatePoint(int,const ScenePos&);

    /**
     * @brief 设置多个位置
     */
    void SetMultPos(const vector<ScenePos>&);
    vector<ScenePos> GetMulPos();

    /**
     * @brief 更新地图节点
     * @param pMapNode
     */
    void UpdateMapNode(osgEarth::MapNode* pMapNode);

    /**
     * @brief 初始化节点
     */
    void InitSceneNode();

    /**
     * @brief 初始化样式
     */
    void InitStyle();

    /**
     * @brief 设置线宽
     */
    void SetLineWidth(float);
    const float& GetLineWidth(){return(m_fLineWidth);}

    /**
     * @brief 设置线型
     */
    void SetLineType(SCENE_LINE_TYPE);
    SCENE_LINE_TYPE GetLineType(){return(m_emType);}

    /**
     * @brief 设置线的颜色
     */
    void SetColor(const SceneColor&);
    const SceneColor& GetColor(){return(m_stColor);}
private:
    float m_fLineWidth=1.0f;
    int   m_nIndex=0;
    LINE_CHANGE_TYPE m_emChangeType=NO_CHANGE; /// 操作类型
    SceneColor m_stColor;
    SCENE_LINE_TYPE m_emType=SOLID_LINE;
    list<ScenePos> m_listAllPos;
    osg::ref_ptr<LineCallBack>          m_pLineCallBack;
    osg::ref_ptr<osgEarth::FeatureNode> m_pFeatureNode;
};

#endif // CSCENELINE_H
