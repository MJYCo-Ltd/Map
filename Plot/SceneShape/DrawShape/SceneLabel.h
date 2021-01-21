#ifndef CSCENELABEL_H
#define CSCENELABEL_H
#include <osgEarth/Text>

#include <Plot/SceneShape/ILabel.h>
#include <Inner/ImplSceneNode.hpp>

class CSceneLabel:public ImplSceneNode<ILabel>,public ISceneNodeAttach
{
public:
    CONSTRUCTOR(CSceneLabel,ImplSceneNode<ILabel>)

    /**
     * @brief 设置依附节点
     */
    bool SetAttachNode(ISceneNode* pSceneNode);
    bool DisAttachNode(ISceneNode* pSceneNode);

    void NodeVisibleChanged(bool);
    void AddIntoParent(osg::Group*);
    void DelFromParent(osg::Group*);

protected:
    void InitNode();
    void UpdateNode();

    void FontChanged()SET_TRUE_NODE_UPDATE(m_bFontChanged)
    void InfoChanged()SET_TRUE_NODE_UPDATE(m_bInfoChanged)
    void PosChanged()SET_TRUE_NODE_UPDATE(m_bPosChanged)
    void BoundingBoxVisibleChanged()SET_TRUE_NODE_UPDATE(m_bBoundingBoxVisibleChanged)
    void FontSizeChanged()SET_TRUE_NODE_UPDATE(m_bFontSizeChanged)
    void FillColorChanged()SET_TRUE_NODE_UPDATE(m_bFillColorChanged)
    void OutColorChanged()SET_TRUE_NODE_UPDATE(m_bOutColorChanged)
    void BoundingBoxColorChanged()SET_TRUE_NODE_UPDATE(m_bBoudingBoxColorChanged)
protected:
    bool m_bInfoChanged=false;
    bool m_bBoundingBoxVisibleChanged=false;
    bool m_bFontSizeChanged=false;
    bool m_bFillColorChanged=false;
    bool m_bOutColorChanged=false;
    bool m_bBoudingBoxColorChanged=false;
    bool m_bPosChanged=false;
    bool m_bFontChanged=false;
    osg::observer_ptr<osgEarth::Text> m_pText;
};

#endif // CSCENELABEL_H
