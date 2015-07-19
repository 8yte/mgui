#ifndef MGUIAPPLICATION_H_
#define MGUIAPPLICATION_H_

#include <core/Application.h>
#include "StatusBar.h"
#include "BottomBar.h"

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

    ilixi::PushButton* _resetButton;
    ilixi::PushButton* _fotaButton;
    ilixi::PushButton* _assertButton;
    ilixi::PushButton* _keepAliveButton;

    ilixi::ToolButton* _cellular;
    ilixi::ToolButton* _wireless;
};

} /* namespace MGUI */

#endif /* MGUIAPPLICATION_H_ */
