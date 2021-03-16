#ifndef CSIMPLELABEL_H
#define CSIMPLELABEL_H
#include <SceneGraph/IWindow.h>
#include <Plot/Map/MapType.h>
#include<SceneGraph/SceneType.h>
#include<string>
class IPlotBase;
class ILabel;
class ILine;
class ISceneGraph;
class ISceneGroup;
class IMapLocation;
class CSimpleLabel :public IWindowMessageObserver
{
public:
    CSimpleLabel(ISceneGraph*);
    ~CSimpleLabel();
   // virtual void MouseDown(MouseButtonMask, int, int) override;
    virtual void MouseMove(MouseButtonMask, int, int) override;
   // virtual void MouseDblClick(MouseButtonMask, int, int) override;


    IMapLocation* getNode();
    void setSimpleLabel(const char* );
    void setComplexLabel(const char* );
    void SetTarget(IPlotBase*);
    void setColor(long );

    void update();

    bool JudgeID(int unSelected,unsigned int);
private:
    std::string simpleLabel;
    std::string fullLabel;
    IPlotBase*  m_pTarget;
    ILabel*     m_pLabel;
    ILine*      m_pLine;
    ISceneGraph* m_pSceneGraph;

    bool m_bLabelChange;
    bool m_bSimpleLabelShow;
    std::string m_CurLabel;
    ISceneGroup*    m_pSceneGroup;
    IMapLocation* m_pRoot;

    MapGeoPos m_TargetPos;

    long    m_LabelColor;
    long    m_LineColor;
    double m_TX;
    double m_TY;
    double m_TZ;

    SceneColor m_Color;
    bool m_bSelect;
};

#endif // CSIMPLELABEL_H
