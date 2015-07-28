#include "MGuiApplication.h"
#include <ui/PushButton.h>
#include <ui/ToolButton.h>
#include <ui/Dialog.h>
#include <core/Logger.h>

namespace MGUI
{

D_DEBUG_DOMAIN(MGUI_APP, "Mgui/Application", "Mgui App");

ilixi::IconPack* MGuiApplication::__iconPack = NULL;

MGuiApplication::MGuiApplication(int argc, char* argv[])
	: Application(&argc, &argv)
{
	__iconPack = new ilixi::IconPack();
	__iconPack->parseIcons(DATADIR"/themes/mgui/IconPack.xml");

	setBackgroundImage(DATADIR"/themes/mgui/bg.dfiff");
	_statusBar = new StatusBar();
	_statusBar->setGeometry(5, 0, 470, 24);
	addWidget(_statusBar);

	_bottomBar = new BottomBar();
	_bottomBar->setGeometry(0, 248, 480, 24);
	addWidget(_bottomBar);

	_resetButton = new ilixi::PushButton("Reset");
	_resetDialog = new ilixi::Dialog("Reset", ilixi::Dialog::OKCancelButtonOption);
	_resetButton->sigReleased.connect(sigc::mem_fun(_resetDialog, &ilixi::Dialog::execute));
	_resetButton->setGeometry(5, 30, 160, 38);
	addWidget(_resetButton);

	_fotaButton = new ilixi::PushButton("Fota");
	_fotaDialog = new ilixi::Dialog("Fota", ilixi::Dialog::OKCancelButtonOption);
	_fotaButton->sigReleased.connect(sigc::mem_fun(_fotaDialog, &ilixi::Dialog::execute));
	_fotaButton->setGeometry(5, 73, 160, 38);
	addWidget(_fotaButton);

	_assertButton = new ilixi::PushButton("No Data Assert");
	_assertDialog = new ilixi::Dialog("No Data Assert", ilixi::Dialog::OKCancelButtonOption);
	_assertButton->sigReleased.connect(sigc::mem_fun(_assertDialog, &ilixi::Dialog::execute));
	_assertButton->setGeometry(5, 116, 160, 38);
	addWidget(_assertButton);

	_keepAliveButton = new ilixi::PushButton("Keep Alive");
	_keepAliveDialog = new ilixi::Dialog("Keep Alive", ilixi::Dialog::OKCancelButtonOption);
	_keepAliveButton->sigReleased.connect(sigc::mem_fun(_keepAliveDialog, &ilixi::Dialog::execute));
	_keepAliveButton->setGeometry(5, 159, 160, 38);
	addWidget(_keepAliveButton);

	_forceUploadeButton = new ilixi::PushButton("Force Upload");
	_forceUploadDialog = new ilixi::Dialog("Force Upload", ilixi::Dialog::OKCancelButtonOption);
	_forceUploadeButton->sigReleased.connect(sigc::mem_fun(_forceUploadDialog, &ilixi::Dialog::execute));
	_forceUploadeButton->setGeometry(5, 202, 160, 38);
	addWidget(_forceUploadeButton);

	_cellular = new ilixi::ToolButton("Cellular");
	_cellular->setGeometry(315, 48, 160, 80);
	//_cellular->setIcon(__iconPack->getIcon("cellular_4_bar")->getImagePath());
	addWidget(_cellular);

	_wireless = new ilixi::ToolButton("Wireless");
	//_wireless->setIcon(__iconPack->getIcon("wifi_4_bar")->getImagePath());
	_wireless->setGeometry(315, 138, 160, 80);
	addWidget(_wireless);

#ifdef PXA1826
	/* init pipe */
	if (pipe(_fd))
		ILOG_THROW(MGUI_APP, "pipe failed with error\n");
	_ubus = new UBusThread(_statusBar, _bottomBar, _cellular, _wireless, _fd[0]);
	_ubus->start();
	_onkey = new OnkeyThread(_fd[1]);
	_onkey->sigOnkeyPress.connect(sigc::mem_fun(this, &MGuiApplication::MGuiStateToggle));
	_onkey->start();

	_resetDialog->sigAccepted.connect(sigc::mem_fun(MGuiHawk::Instance(), &MGuiHawk::Reset));
	_fotaDialog->sigAccepted.connect(sigc::mem_fun(MGuiHawk::Instance(), &MGuiHawk::Fota));
	_assertDialog->sigAccepted.connect(sigc::mem_fun(MGuiHawk::Instance(), &MGuiHawk::Assert));
	_keepAliveDialog->sigAccepted.connect(sigc::mem_fun(MGuiHawk::Instance(), &MGuiHawk::KeepAlive));
	_forceUploadDialog->sigAccepted.connect(sigc::mem_fun(MGuiHawk::Instance(), &MGuiHawk::ForceUpload));

#endif

	_timer = new ilixi::Timer();
	_timer->sigExec.connect(sigc::mem_fun(this, &MGuiApplication::MGuiStateToggle));
	_timer->start(30000);

	MGuiSetState(MGuiAppStateOn);
}

MGuiApplication::~MGuiApplication()
{
#ifdef PXA1826
	MGuiHawk::Destroy();
	_onkey->cancel();
	_ubus->cancel();
	close(_fd[0]);
	close(_fd[1]);
	delete _ubus;
	delete _onkey;
#endif
	Touch(false);
	Screen(false);
}

#ifdef PXA1826
#define TOUCH_RUNTIME_PM_PATH "/sys/bus/i2c/drivers/cyttsp-i2c/0-0024/power/control"
#else
#define TOUCH_RUNTIME_PM_PATH ""
#endif

void
MGuiApplication::Touch(bool on)
{
	int ret = 0;
	const char *mode[] = {"auto", "on"};

	ILOG_TRACE(MGUI_APP);

	int fd = open(TOUCH_RUNTIME_PM_PATH, O_RDWR);

	if (fd < 0) {
		ILOG_ERROR(MGUI_APP, "Can't open touchscreen runtime control (%s)\n", TOUCH_RUNTIME_PM_PATH);
		return;
	}

	ret = write(fd, mode[!!on], strlen(mode[!!on]));
	if (ret < 0) {
		ILOG_ERROR(MGUI_APP, "%s runtime suspend failed\n", on ? "disable" : "enable");
		goto out;
	}

	ILOG_DEBUG(MGUI_APP, "%s runtime suspend success\n", on ? "disable" : "enable");
out:
	close(fd);
	ILOG_DEBUG(MGUI_APP, "%s: Exit\n", __func__);
}

void
MGuiApplication::Screen(bool on)
{
	IDirectFB *dfb = ilixi::PlatformManager::instance().getDFB();
	IDirectFBScreen *screen;

	ILOG_TRACE(MGUI_APP);

	dfb->GetScreen(dfb, DSCID_PRIMARY, &screen);
	screen->SetPowerMode(screen, on ? DSPM_ON : DSPM_OFF);
	screen->Release(screen);

	ILOG_DEBUG(MGUI_APP, "%s: Exit\n", __func__);
}


void
MGuiApplication::MGuiSetState(const MguiApplicationState &state)
{
	ILOG_TRACE(MGUI_APP);

	if (state == MGuiAppStateOn) {
		Screen(true);
		Touch(true);
		_state = MGuiAppStateOn;
		_timer->restart();
	} else {
		Screen(false);
		Touch(false);
		_state = MGuiAppStateOff;
		_timer->stop();
	}

	ILOG_DEBUG(MGUI_APP, "%s: Exit\n", __func__);
}

void
MGuiApplication::MGuiStateToggle()
{
	ILOG_TRACE(MGUI_APP);

	if (_state == MGuiAppStateOn) {
		ILOG_DEBUG(MGUI_APP, "Going to sleep...\n");
		MGuiSetState(MGuiAppStateOff);
	} else {
		ILOG_DEBUG(MGUI_APP, "Waking up...\n");
		MGuiSetState(MGuiAppStateOn);
	}

	ILOG_DEBUG(MGUI_APP, "%s: Exit\n", __func__);
}


bool
MGuiApplication::windowPreEventFilter(const DFBWindowEvent& event)
{
	if (_state == MGuiAppStateOn) {
		ILOG_DEBUG(MGUI_APP, "%s: reschedule timer\n", __func__);
		_timer->restart();
	}
	return false;
}

} /* namespace MGUI */
