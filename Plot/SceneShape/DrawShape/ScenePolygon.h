#ifndef CSCENEPOLYGON_H
#define CSCENEPOLYGON_H

#include <list>
#include <osgUtil/Tessellator>
#include <Plot/SceneShape/IPolygon.h>
#include <Inner/ImplSceneShape.hpp>

class CScenePolygon:public ImplSceneShape<IPolygon>
{
public:
    CONSTRUCTOR(CScenePolygon,ImplSceneShape<IPolygon>)

    /**
     * @brief增加一个点
     */
    void AddPoint(int,const ScenePos&);

    /**
     * @brief 移除一个点
     * @return
     */
    bool RemovePoint(int);

    /**
     * @brief更新点
     * @return
     */
    bool UpdatePoint(int,const ScenePos&);

    /**
     * @brief 一次设置多个点
     */
    void SetMultPos(const std::vector<ScenePos>&);
    std::vector<ScenePos> GetMulPos()const;
protected:
    void CreateShape();
    void UpdateShape();
protected:
    osg::ref_ptr<osg::DrawArrays> m_pDrawArrays;
    osg::ref_ptr<osgUtil::Tessellator> m_pTess;
    std::list<ScenePos>    m_listAllPos;
};

#endif // CSCENEPOLYGON_H
