#ifndef MGUIAPPLICATION_H_
#define MGUIAPPLICATION_H_

#include <core/Application.h>
#include "StatusBar.h"
#include "BottomBar.h"
#ifdef PXA1826
#include <platform/pxa1826/UBusThread.h>
#endif

namespace ilixi
{
class PushButton;
class ToolButton;
}

namespace MGUI
{

class MGuiApplication : public ilixi::Application
{
public:
    MGuiApplication(int argc, char* argv[]);

    virtual
    ~MGuiApplication();

private:
    StatusBar* _statusBar;
    BottomBar* _bottomBar;
#ifdef PXA1826
    UBusThread* _thread;
#endif
    ilixi::PushButton* _resetButton;
    ilixi::PushButton* _fotaButton;
    ilixi::PushButton* _assertButton;
    ilixi::PushButton* _keepAliveButton;

    ilixi::ToolButton* _cellular;
    ilixi::ToolButton* _wireless;

};

} /* namespace MGUI */

#endif /* MGUIAPPLICATION_H_ */
