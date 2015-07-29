#include "HawkScreen.h"

#include <ui/PushButton.h>
#include <ui/ToolButton.h>
#include <ui/Dialog.h>

namespace MGUI
{

HawkScreen::HawkScreen()
        : ScreenBase()
{
	_title = new ilixi::Label("Hawk");
	_title->setGeometry(0, 0, 480, 50);
	addChild(_title);
	
	_resetButton = new ilixi::PushButton("Reset");
	_resetDialog = new ilixi::Dialog("Reset", ilixi::Dialog::OKCancelButtonOption);
	_resetButton->sigReleased.connect(sigc::mem_fun(_resetDialog, &ilixi::Dialog::execute));
	_resetButton->setGeometry(5, 10, 160, 40);
	addChild(_resetButton);

	_fotaButton = new ilixi::PushButton("Fota");
	_fotaDialog = new ilixi::Dialog("Fota", ilixi::Dialog::OKCancelButtonOption);
	_fotaButton->sigReleased.connect(sigc::mem_fun(_fotaDialog, &ilixi::Dialog::execute));
	_fotaButton->setGeometry(5, 53, 160, 40);
	addChild(_fotaButton);

	_assertButton = new ilixi::PushButton("No Data Assert");
	_assertDialog = new ilixi::Dialog("No Data Assert", ilixi::Dialog::OKCancelButtonOption);
	_assertButton->sigReleased.connect(sigc::mem_fun(_assertDialog, &ilixi::Dialog::execute));
	_assertButton->setGeometry(5, 96, 160, 40);
	addChild(_assertButton);

	_keepAliveButton = new ilixi::PushButton("Keep Alive");
	_keepAliveDialog = new ilixi::Dialog("Keep Alive", ilixi::Dialog::OKCancelButtonOption);
	_keepAliveButton->sigReleased.connect(sigc::mem_fun(_keepAliveDialog, &ilixi::Dialog::execute));
	_keepAliveButton->setGeometry(5, 139, 160, 40);
	addChild(_keepAliveButton);

	_forceUploadeButton = new ilixi::PushButton("Force Upload");
	_forceUploadDialog = new ilixi::Dialog("Force Upload", ilixi::Dialog::OKCancelButtonOption);
	_forceUploadeButton->sigReleased.connect(sigc::mem_fun(_forceUploadDialog, &ilixi::Dialog::execute));
	_forceUploadeButton->setGeometry(5, 182, 160, 40);
	addChild(_forceUploadeButton);

#ifdef PXA1826
	_resetDialog->sigAccepted.connect(sigc::mem_fun(MGuiHawk::Instance(), &MGuiHawk::Reset));
	_fotaDialog->sigAccepted.connect(sigc::mem_fun(MGuiHawk::Instance(), &MGuiHawk::Fota));
	_assertDialog->sigAccepted.connect(sigc::mem_fun(MGuiHawk::Instance(), &MGuiHawk::Assert));
	_keepAliveDialog->sigAccepted.connect(sigc::mem_fun(MGuiHawk::Instance(), &MGuiHawk::KeepAlive));
	_forceUploadDialog->sigAccepted.connect(sigc::mem_fun(MGuiHawk::Instance(), &MGuiHawk::ForceUpload));
#endif
}

HawkScreen::~HawkScreen()
{
	delete _resetDialog;
	delete _fotaDialog;
	delete _assertDialog;
	delete _keepAliveDialog;
	delete _forceUploadDialog;
}

} /* namespace MGUI */
