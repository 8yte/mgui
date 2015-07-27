#include "MGuiApplication.h"
#include <ui/PushButton.h>
#include <ui/ToolButton.h>
#include <core/Logger.h>

namespace MGUI
{

D_DEBUG_DOMAIN(MGUI_APP, "Mgui/Application", "Mgui App");

MGuiApplication::MGuiApplication(int argc, char* argv[])
	: Application(&argc, &argv)
{
	setBackgroundImage(DATADIR"/themes/mgui/bg.dfiff");
	_statusBar = new StatusBar();
	_statusBar->setGeometry(5, 0, 470, 24);
	addWidget(_statusBar);

	_bottomBar = new BottomBar();
	_bottomBar->setGeometry(0, 248, 480, 24);
	addWidget(_bottomBar);

	_resetButton = new ilixi::PushButton("Reset");
	_resetButton->setGeometry(5, 48, 160, 40);
	addWidget(_resetButton);

	_fotaButton = new ilixi::PushButton("Fota");
	_fotaButton->setGeometry(5, 93, 160, 40);
	addWidget(_fotaButton);

	_assertButton = new ilixi::PushButton("No data assert");
	_assertButton->setGeometry(5, 138, 160, 40);
	addWidget(_assertButton);

	_keepAliveButton = new ilixi::PushButton("Keep alive");
	_keepAliveButton->setGeometry(5, 183, 160, 40);
	addWidget(_keepAliveButton);

	_cellular = new ilixi::ToolButton("Cellular");
	_cellular->setGeometry(315, 48, 160, 80);
	addWidget(_cellular);

	_wireless = new ilixi::ToolButton("Wireless");
	_wireless->setGeometry(315, 138, 160, 80);
	addWidget(_wireless);

#ifdef PXA1826
	/* init pipe */
	if (pipe(_fd))
		ILOG_THROW(MGUI_APP, "pipe failed with error\n");
	_ubus = new UBusThread(_statusBar, _bottomBar, _fd[0]);
	_ubus->start();
	_onkey = new OnkeyThread(_fd[1]);
	_onkey->sigOnkeyPress.connect(sigc::mem_fun(this, &MGuiApplication::MGuiStateChange));
	_onkey->start();
#endif

	_timer = new ilixi::Timer();
	_timer->sigExec.connect(sigc::mem_fun(this, &MGuiApplication::MGuiStateChange));
	_timer->start(30000);
}

MGuiApplication::~MGuiApplication()
{
#ifdef PXA1826
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
MGuiApplication::MGuiStateChange()
{
	ILOG_TRACE(MGUI_APP);

	if (_state == MGuiAppStateOn) {
		ILOG_DEBUG(MGUI_APP, "Going to sleep...\n");
		Screen(false);
		Touch(false);
		_state = MGuiAppStateOff;
		_timer->stop();
	} else {
		ILOG_DEBUG(MGUI_APP, "Waking up...\n");
		Screen(true);
		Touch(true);
		_state = MGuiAppStateOn;
		_timer->restart();
	}

	ILOG_DEBUG(MGUI_APP, "%s: Exit\n", __func__);
}

} /* namespace MGUI */
