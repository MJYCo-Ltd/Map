#ifndef CSCENELINE_H
#define CSCENELINE_H
#include <list>
#include <Plot/SceneShape/ILine.h>
#include <Inner/ImplSceneShape.hpp>
#include <osg/Uniform>
class CSceneLine:public ImplSceneShape<ILine>
{
public:
    CONSTRUCTOR(CSceneLine,ImplSceneShape<ILine>)

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
    std::list<ScenePos>           m_listAllPos;
    osg::ref_ptr<osg::Uniform>    m_pUniform;
};

#endif // CSCENELINE_H
