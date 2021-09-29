#ifndef CSCENEPOLYGON3D_H
#define CSCENEPOLYGON3D_H

#include <Plot/SceneShape/IPolygon3D.h>
#include <Inner/ImplSceneGeometry.hpp>

class CScenePolygon3D:public ImplSceneGeometry<IPolygon3D>
{
public:
    CONSTRUCTOR(CScenePolygon3D,ImplSceneGeometry<IPolygon3D>);
protected:
    void HeightChanged()SET_TRUE_SHAPE_UPDATE(m_bHeightChanged)

    /**
     * @brief 重写父类方法
     */
    void CreateShape();
    void UpdateShape();

    /**
     * @brief 更新上面的点
     */
    void UpdateTopFaceVertex();
protected:
    bool m_bHeightChanged{false};
};

#endif // CSCENEPOLYGON3D_H
