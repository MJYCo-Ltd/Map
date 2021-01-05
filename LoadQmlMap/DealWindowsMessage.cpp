#include "DealWindowsMessage.h"

CDealWindowsMessage::CDealWindowsMessage()
{

}

#include <QDebug>
void CDealWindowsMessage::MouseDown(MouseButtonMask, int, int)
{
    qDebug()<<"hello Mouse Dow";
}
