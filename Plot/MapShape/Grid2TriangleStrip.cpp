#include "Grid2TriangleStrip.h"

CGrid2TriangleStrip::CGrid2TriangleStrip(unsigned int unRow, unsigned int unLine, bool bCloseure):
    m_unRowCount(unRow),
    m_unLineCount(unLine),
    m_bClosure(bCloseure)
{
    GenerateIndex();
}

/// 设置行数和列数
void CGrid2TriangleStrip::SetRowAndLine(unsigned int unRow, unsigned int unLine)
{
    /// 如果行列数相同则退出
    if(unRow == m_unRowCount && unLine == m_unLineCount)
    {
        return;
    }

    m_unRowCount = unRow;
    m_unLineCount = unLine;

    /// 生成索引数组
    GenerateIndex();
}

/// 设置行尾部是否闭合
void CGrid2TriangleStrip::SetClosure(bool bClosure)
{
    /// 如果相同则退出
    if(bClosure == m_bClosure)
    {
        return;
    }

    m_bClosure = bClosure;

    /// 如果状态改该则重新生成索引
    GenerateIndex();
}

#include <iostream>
/// 生成顶点索引数组
void CGrid2TriangleStrip::GenerateIndex()
{
    m_vIndex.clear();

    /// 根据收尾是否相接进行数据的产生
    osg::VectorGLuint tmpVIndex;
    if(m_bClosure)
    {
        tmpVIndex.resize(2*(m_unLineCount+1));
    }
    else
    {
        tmpVIndex.resize(2*m_unLineCount);
    }

    int nBase,nIndexBase;

    /// 开始构建索引数组
    for(int nIndexRow = 0; nIndexRow < m_unRowCount; ++nIndexRow)
    {
        if(m_unRowCount -1 != nIndexRow)
        {
            /// 如果闭合
            if(m_bClosure)
            {
                nIndexBase = 2*nIndexRow * (m_unLineCount + 1);
            }
            else
            {
                nIndexBase = 2*nIndexRow * m_unLineCount;
            }

            nBase = (m_unRowCount - 1 - nIndexRow) * m_unLineCount;
            /// 循环生成
            for(int nIndexLine = 0; nIndexLine < m_unLineCount; ++nIndexLine)
            {
                tmpVIndex[nIndexLine*2] = nBase + nIndexLine;
                tmpVIndex[1 + nIndexLine*2] = nBase + nIndexLine - m_unLineCount;
            }

            /// 如果闭合则多增加两个
            if(m_bClosure)
            {
                tmpVIndex[m_unLineCount*2] = nBase;
                tmpVIndex[m_unLineCount*2 + 1] = nBase - m_unLineCount;
            }

            m_vIndex.push_back(tmpVIndex);
        }
    }

    int i=0;
}
