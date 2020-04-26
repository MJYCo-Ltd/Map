#ifndef QT_OSG_USER_ITEM_H
#define QT_OSG_USER_ITEM_H

/**
 * @brief Qt Quick 用户自定义的场景节点
 */

#include "QtOsgItem.h"


class SCENECORESHARED_EXPORT QtOsgUserItem:public QtOsgItem
{
    Q_OBJECT
public:
    QtOsgUserItem();
    ~QtOsgUserItem();
};

#endif // OSGITEM_H
