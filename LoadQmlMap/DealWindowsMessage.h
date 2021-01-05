#ifndef CDEALWINDOWSMESSAGE_H
#define CDEALWINDOWSMESSAGE_H
#include <SceneGraph/IWindow.h>

class CDealWindowsMessage:public IWindowMessageObserver
{
public:
    CDealWindowsMessage();

    void MouseDown(MouseButtonMask, int, int);
};

#endif // CDEALWINDOWSMESSAGE_H
