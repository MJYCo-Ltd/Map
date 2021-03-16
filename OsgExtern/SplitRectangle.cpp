#include <Inner/OsgExtern/SplitRectangle.h>

CSplitRectangle::CSplitRectangle()
{
}

void CSplitRectangle::SetRectangle(const Rectangle &rRectangle)
{
    m_stRectangle = rRectangle;
    ReCal();
}

void CSplitRectangle::ReCal()
{
}
