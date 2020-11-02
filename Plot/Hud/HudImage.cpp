#include "HudImage.h"

void CHudImage::ImageChanged()
{
    m_bImageChanged = true;
    NodeChanged();
}
