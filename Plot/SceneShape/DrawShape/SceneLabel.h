#ifndef CSCENELABEL_H
#define CSCENELABEL_H
#include <osgText/Text>

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

    void FontChanged(){m_bFontChanged=true;ImplSceneNode<ILabel>::NodeChanged();}
    void InfoChanged(){m_bInfoChanged=true;ImplSceneNode<ILabel>::NodeChanged();}
    void PosChanged(){m_bPosChanged=true;ImplSceneNode<ILabel>::NodeChanged();}
    void BoundingBoxVisibleChanged(){m_bBoundingBoxVisibleChanged=true;ImplSceneNode<ILabel>::NodeChanged();}
    void FontSizeChanged(){m_bFontSizeChanged=true;ImplSceneNode<ILabel>::NodeChanged();}
    void FillColorChanged(){m_bFillColorChanged=true;ImplSceneNode<ILabel>::NodeChanged();}
    void OutColorChanged(){m_bOutColorChanged=true;ImplSceneNode<ILabel>::NodeChanged();}
    void BoundingBoxColorChanged(){m_bBoudingBoxColorChanged=true;ImplSceneNode<ILabel>::NodeChanged();}
protected:
    bool m_bInfoChanged=false;
    bool m_bBoundingBoxVisibleChanged=false;
    bool m_bFontSizeChanged=false;
    bool m_bFillColorChanged=false;
    bool m_bOutColorChanged=false;
    bool m_bBoudingBoxColorChanged=false;
    bool m_bPosChanged=false;
    bool m_bFontChanged=false;
    osg::observer_ptr<osgText::Text> m_pText;
};

#endif // CSCENELABEL_H
