#ifndef H_HEADER_MATHCOMMON_H
#define H_HEADER_MATHCOMMON_H

#include "OsgExtern_Global.h"

class OSGEXTERNSHARED_EXPORT CMathCommon
{
public:

    static CMathCommon* GetInstance();
    /**
     * @brief 获取sin值
     * @return
     */
    const double* GetSin();

    /**
     * @brief 获取cos值
     * @return
     */
    const double* GetCos();
private:
    CMathCommon();
    ~CMathCommon();

    /**
     * @brief 初始化数据
     */
    void Init();

private:
    double* m_pSin;    /// sin值
    double* m_pCos;  /// cos值
    bool       m_bInit;    /// 是否初始化
};

#endif // SNGEOMATHCOMMON_H
