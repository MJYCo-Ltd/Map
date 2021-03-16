#ifndef SPLITRECTANGLE_HEADER_H
#define SPLITRECTANGLE_HEADER_H

#include <Inner/OsgExtern/OsgExtern_Global.h>

struct RectanglePos
{
    double dLon;
    double dLat;
};

struct Rectangle
{
    RectanglePos LeftUp;
    RectanglePos RightDown;
};

/**
 * @brief将矩形划分成点序列
 */
class OSGEXTERNSHARED_EXPORT CSplitRectangle
{
public:
    CSplitRectangle();

    /**
     * @brief 设置矩形
     * @param rRectangle
     */
    void SetRectangle(const Rectangle& rRectangle);

    void SetGis();
protected:
    void ReCal();
protected:
    Rectangle m_stRectangle;
};

#endif // SPLITRECTANGLE_HEADER_H
