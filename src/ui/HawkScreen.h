#ifndef SRC_UI_HAWKSCREEN_H_
#define SRC_UI_HAWKSCREEN_H_

#include "ScreenBase.h"

#include <ui/Label.h>

namespace ilixi
{
class PushButton;
class Dialog;
}

namespace MGUI
{

class HawkScreen : public ScreenBase
{
public:
	HawkScreen();

	virtual
	~HawkScreen();

private:
	ilixi::Label* _title;
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
};

} /* namespace MGUI */

#endif /* SRC_UI_HAWKSCREEN_H_ */
