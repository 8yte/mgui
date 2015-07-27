#ifndef MGUIAPPLICATION_H_
#define MGUIAPPLICATION_H_

#include <core/Application.h>
#include <core/PlatformManager.h>
#include <ui/StatusBar.h>
#include <ui/BottomBar.h>
#ifdef PXA1826
#include <platform/pxa1826/UBusThread.h>
#include <platform/pxa1826/OnkeyThread.h>
//#include <ril.h>
//#include <rilutil.h>
#include <platform/pxa1826/MGuiRil.h>
#include <platform/pxa1826/MGuiCharger.h>
#include <platform/pxa1826/MGuiWifi.h>
#include <platform/pxa1826/MGuiStats.h>
#include <platform/pxa1826/MGuiHawk.h>
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

    void
    MGuiStateChange();

private:
    void
    Screen(bool on);

    void
    Touch(bool on);

    enum MguiApplicationState {
        MGuiAppStateOn,
        MGuiAppStateOff
    } _state;
    StatusBar* _statusBar;
    BottomBar* _bottomBar;

#ifdef PXA1826
    int _fd[2];
    UBusThread* _ubus;
    OnkeyThread* _onkey;
#endif
    ilixi::PushButton* _resetButton;
    ilixi::PushButton* _fotaButton;
    ilixi::PushButton* _assertButton;
    ilixi::PushButton* _keepAliveButton;

    ilixi::ToolButton* _cellular;
    ilixi::ToolButton* _wireless;

    ilixi::Timer* _timer;
};

} /* namespace MGUI */

#endif /* MGUIAPPLICATION_H_ */
