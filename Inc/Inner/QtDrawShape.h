#ifndef QT_DRAW_SHAPE_H
#define QT_DRAW_SHAPE_H
#include <osgEarth/Style>
#include "QtOsgEarthMapSceneNode.h"

template <typename T>
class QtDrawShape:public QtOsgEarthMapSceneNode<T>
{
public:
    explicit QtDrawShape(ISceneGraph* pSceneGraph):QtOsgEarthMapSceneNode<T>(pSceneGraph){}
    virtual ~QtDrawShape(){}

    /**
     * @brief 判断ID是否有效
     * @return
     */
    bool JudgeID(unsigned)
    {
        return(false);
    }

    void InitSceneNode()
    {
        InitStyle();
        QtOsgEarthMapSceneNode<T>::InitSceneNode();
    }

protected:

    /**
     * @brief 初始化样式
     */
    virtual void InitStyle(){}
    /**
     * @brief 贴地方式修改
     */
    void TerrainTypeChanged()
    {
        switch (QtOsgEarthMapSceneNode<T>::m_emTerrain)
        {
        case CLOSE_TERRAIN:
            m_styleNode.getOrCreate<osgEarth::AltitudeSymbol>()
                    ->clamping()=osgEarth::AltitudeSymbol::CLAMP_TO_TERRAIN;
            break;
        case RELATIVE_TERRAIN:
            m_styleNode.getOrCreate<osgEarth::AltitudeSymbol>()
                    ->clamping()=osgEarth::AltitudeSymbol::CLAMP_RELATIVE_TO_TERRAIN;
            break;
        }
    }

protected:
    osgEarth::Style   m_styleNode;  /// 节点样式
};

#endif // QT_OSGEARTH_MAP_SCENE_NODE_H
