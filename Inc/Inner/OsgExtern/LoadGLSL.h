#ifndef LOAD_GLSL_HEADER_H
#define LOAD_GLSL_HEADER_H
#include <string>
#include <unordered_map>
#include <osg/StateSet>
#include <Inner/OsgExtern/OsgExtern_Global.h>

/**
 * @brief 字段类型
 */
enum FIELD_TYPE
{
    FIELD_FLOAT,
    FIELD_VEC2,
    FIELD_VEC4,
    FIELD_INT,
    FIELD_BOOL
};

struct F_VEC2
{
    float fV1;
    float fV2;
};

struct F_VEC4
{
    F_VEC2 v1;
    F_VEC2 v2;
};

/**
 * @brief 字段值
 */
union FieldValue
{
    bool bValue;
    int  nValue;
    float fValue;
    F_VEC2 stV2;
    F_VEC4 stV4;
};

/**
 * @brief 字段
 */
struct Field
{
    FIELD_TYPE type;
    FieldValue value;
    std::string ToString()
    {
        switch (type)
        {
        case FIELD_INT:
//            return(itoa(value.nValue,10));
        case FIELD_FLOAT:
            break;
        case FIELD_BOOL:
            if(value.bValue == false)
            {
                return "_flase";
            }
            else
            {
                return "_true";
            }
        case FIELD_VEC2:
            break;
        case FIELD_VEC4:
            break;
        }
    }
};

/**
 * @brief 负责加载glsl文件的类
 */
class OSGEXTERNSHARED_EXPORT CLoadGLSL
{
public:
    CLoadGLSL(const std::string& sGlslFileName, const std::string& sGlslConfigFileName);

    /**
     * @brief 加载glsl文件
     */
    void LoadGLSL(osg::State* pSateSet);

    /**
     * @brief 更改指定类型的值
     */
    osg::StateSet* ChangeFieldValue(const std::string& sFieldName,const Field& fieldValue);
protected:
    void LoadGlsl(const std::string& sGlslFileName);
    void LoadConfig(const std::string& sConfigFileName);
private:
    std::set<std::string>                                  m_setField;         /// 作用域的集合
    std::unordered_map<std::string,osg::ref_ptr<osg::StateSet>> m_mapValue2Stateset;/// 对应关系
    osg::ref_ptr<osg::StateSet>                            m_pDefaultStateset; /// 默认的状态集合
};
#endif // LOAD_GLSL_HEADER_H
