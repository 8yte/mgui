#ifndef MGUIAPPLICATION_H_
#define MGUIAPPLICATION_H_

#include <core/Application.h>
#include <core/PlatformManager.h>
#include <ui/ToolButton.h>

#include "StatusBar.h"
#include "HomeScreen.h"
#include "HawkScreen.h"
#include "StatScreen.h"

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

    void
    switchScreen(int screen);

    static ilixi::IconPack* __iconPack;
    MguiApplicationState _state;
    StatusBar* _statusBar;

#ifdef PXA1826
    int _fd[2];
    UBusThread* _ubus;
    OnkeyThread* _onkey;
#endif

    ilixi::ToolButton* _homeButton;
    ilixi::ToolButton* _hawkButton;
    ilixi::ToolButton* _statButton;

    HomeScreen* _homeScreen;
    HawkScreen* _hawkScreen;
    StatScreen* _statScreen;

    ilixi::Timer* _timer;
};

} /* namespace MGUI */

#endif /* MGUIAPPLICATION_H_ */
