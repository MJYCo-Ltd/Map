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

    enum MineType
    {
        MINE_OK,         /// 地雷完好
        MINE_DISARM,     /// 地雷撤防
        MINE_EXPLOSION   /// 地雷爆炸
    };

    void SetType(MineType emType)JUDGE_EQUAL_CALL_FUNCTION(emType,m_emType,TypeChanged)
    MineType Type()const{return(m_emType);}

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
    virtual void TypeChanged()=0;

    virtual ~IMineInfo(){}
protected:
    std::string m_sName;  /// 显示的内容
    MapGeoPos   m_geoPos; /// 位置
    MineType    m_emType=MINE_OK; /// 当前状态
    SceneColor  m_stColor;/// 颜色
};

#endif
