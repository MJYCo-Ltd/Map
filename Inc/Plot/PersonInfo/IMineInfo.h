#ifndef INTERFACE_MINEINFO_HEARDER_H
#define INTERFACE_MINEINFO_HEARDER_H
#include <string>

#include <Plot/Map/IMapSceneNode.h>

/**
 * @brief 场景节点类
 */
class IMineInfo:public IMapSceneNode
{
public:
    CONSTRUCTOR(IMineInfo,IMapSceneNode)

    /**
     * @brief 设置名字
     */
    void SetName(const std::string& rName)JUDGE_EQUAL_CALL_FUNCTION(rName,m_sName,NameChanged)
    const std::string& GetName()const{return(m_sName);}
	
    /**
     * @brief 设置人员位置
     * @param rGeoPos
     */
    void SetPos(const MapGeoPos& rGeoPos)JUDGE_EQUAL_CALL_FUNCTION(rGeoPos,m_geoPos,PosChanged)
    const MapGeoPos& GetPos()const{return(m_geoPos);}

    /**
     * @brief 设置颜色
     */
    void SetColor(const SceneColor& rColor)JUDGE_EQUAL_CALL_FUNCTION(rColor,m_stColor,ColorChanged)
    const SceneColor& GetColor()const{return(m_stColor);}
protected:
    virtual void PosChanged()=0;
    virtual void NameChanged()=0;
    virtual void ColorChanged()=0;

    virtual ~IMineInfo(){}
protected:
    std::string m_sName;
    MapGeoPos   m_geoPos;
    SceneColor  m_stColor;
};

#endif
