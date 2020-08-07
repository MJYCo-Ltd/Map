#ifndef H_HEADER_CSCENELINE_H
#define H_HEADER_CSCENELINE_H

#include <osgEarth/FeatureNode>
#include <Map/Plot/ILine.h>
#include <Inner/QtDrawShape.h>

class CSceneLine:public QtDrawShape<ILine>
{
public:
    CSceneLine(ISceneGraph*);


    /**
     * @brief 增加点
     */
    virtual void AddPoint(const ScenePos&);

    /**
     * @brief 移除点
     */
    virtual void RemovePoint(int);

    /**
     * @brief 更新指定位置
     */
    virtual void UpdatePoint(int,const ScenePos&);

    /**
     * @brief 设置多个位置
     */
    void SetMultPos(const vector<ScenePos>&);
    const vector<ScenePos>& GetMulPos(){return(m_vAllPos);}

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
    SceneColor m_stColor;
    SCENE_LINE_TYPE m_emType=SOLID_LINE;
    vector<ScenePos> m_vAllPos;
    osg::ref_ptr<osgEarth::FeatureNode> m_pFeatureNode;
};

#endif // CSCENELINE_H
