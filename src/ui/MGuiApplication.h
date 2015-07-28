#ifndef MGUIAPPLICATION_H_
#define MGUIAPPLICATION_H_

#include <core/Application.h>
#include <core/PlatformManager.h>
#include <ui/StatusBar.h>
#include <ui/BottomBar.h>
#ifdef PXA1826
#include <platform/pxa1826/UBusThread.h>
#include <platform/pxa1826/OnkeyThread.h>
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
class Dialog;
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
    enum MguiApplicationState {
        MGuiAppStateOn,
        MGuiAppStateOff
    };

    void
    MGuiStateToggle();

    void
    MGuiSetState(const MguiApplicationState &state);

    virtual bool
    windowPreEventFilter(const DFBWindowEvent& event);

    void
    Screen(bool on);

    void
    Touch(bool on);

    static ilixi::IconPack* __iconPack;
    MguiApplicationState _state;
    StatusBar* _statusBar;
    BottomBar* _bottomBar;

#ifdef PXA1826
    int _fd[2];
    UBusThread* _ubus;
    OnkeyThread* _onkey;
#endif
    ilixi::PushButton* _resetButton;
    ilixi::Dialog* _resetDialog;
    ilixi::PushButton* _fotaButton;
    ilixi::Dialog* _fotaDialog;
    ilixi::PushButton* _assertButton;
    ilixi::Dialog* _assertDialog;
    ilixi::PushButton* _keepAliveButton;
    ilixi::Dialog* _keepAliveDialog;
    ilixi::PushButton* _forceUploadeButton;
    ilixi::Dialog* _forceUploadDialog;

    ilixi::ToolButton* _cellular;
    ilixi::ToolButton* _wireless;

    ilixi::Timer* _timer;
};

} /* namespace MGUI */

#endif /* MGUIAPPLICATION_H_ */
