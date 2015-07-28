#ifndef SRC_UI_HOMESCREEN_H_
#define SRC_UI_HOMESCREEN_H_

#include "ScreenBase.h"

namespace ilixi
{
class PushButton;
class ToolButton;
class Dialog;
}

namespace MGUI
{

class HomeScreen : public ScreenBase
{
public:
    HomeScreen();

    virtual
    ~HomeScreen();

private:
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
};

} /* namespace MGUI */

#endif /* SRC_UI_HOMESCREEN_H_ */
