#ifndef CHEADER_SENEPOLYGON_H
#define CHEADER_SENEPOLYGON_H

#include <osgEarth/FeatureNode>
#include <Map/Plot/IPolygon.h>
#include <Inner/QtDrawShape.h>
#include "SceneLine.h"
class PolygonCallBack;

class CScenePolygon:public QtDrawShape<IPolygon>
{
    friend class PolygonCallBack;
public:
    CScenePolygon(ISceneGraph*);

    /**
     * @brief 增加点
     */
    virtual void AddPoint(int,const ScenePos&);

    /**
     * @brief 移除点
     */
    virtual bool RemovePoint(int);

    /**
     * @brief 更新指定位置点
     */
    virtual bool UpdatePoint(int,const ScenePos&);

    /**
     * @brief 设置多个位置
     */
    virtual void SetMultPos(const vector<ScenePos>&);
    virtual vector<ScenePos> GetMulPos();

    /**
     * @brief 设置点的大小
     */
    virtual void SetColor(const SceneColor&);
    virtual const SceneColor& GetColor();

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
private:
    int   m_nIndex=0;
    LINE_CHANGE_TYPE m_emChangeType=NO_CHANGE; /// 操作类型
    SceneColor m_stColor;
    list<ScenePos> m_listAllPos;
    osg::ref_ptr<PolygonCallBack>      m_pPolygonCallBack;
    osg::ref_ptr<osgEarth::FeatureNode> m_pFeatureNode;
};

#endif // CSENEPOLYGON_H
