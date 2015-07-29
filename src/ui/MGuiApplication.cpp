#include "MGuiApplication.h"

#include <core/Logger.h>
#include <sigc++/bind.h>

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
	_statusBar->setGeometry(210, 224, 270, 48);
	addWidget(_statusBar);

	_homeButton = new ilixi::ToolButton("");
	_homeButton->setGeometry(0, 224, 56, 56);
	_homeButton->setCheckable(true);
	_homeButton->setChecked(true);
	_homeButton->setDrawFrame(false);
	_homeButton->setToolButtonStyle(ilixi::ToolButton::IconAboveText);
	_homeButton->sigClicked.connect(sigc::bind<int>(sigc::mem_fun(this, &MGuiApplication::switchScreen), 0));
	_homeButton->setIcon(DATADIR"/themes/mgui/home.png", ilixi::Size(30, 30));
	addWidget(_homeButton);

	_hawkButton = new ilixi::ToolButton("");
	_hawkButton->setGeometry(59, 224, 56, 56);
	_hawkButton->setCheckable(true);
	_hawkButton->setDrawFrame(false);
	_hawkButton->setToolButtonStyle(ilixi::ToolButton::IconAboveText);
	_hawkButton->sigClicked.connect(sigc::bind<int>(sigc::mem_fun(this, &MGuiApplication::switchScreen), 1));
	_hawkButton->setIcon(DATADIR"/themes/mgui/hawk.png", ilixi::Size(30, 30));
	addWidget(_hawkButton);

	_statButton = new ilixi::ToolButton("");
	_statButton->setGeometry(118, 224, 56, 56);
	_statButton->setCheckable(true);
	_statButton->setDrawFrame(false);
	_statButton->setToolButtonStyle(ilixi::ToolButton::IconAboveText);
	_statButton->sigClicked.connect(sigc::bind<int>(sigc::mem_fun(this, &MGuiApplication::switchScreen), 2));
	_statButton->setIcon(DATADIR"/themes/mgui/statistics.png", ilixi::Size(30, 30));
	addWidget(_statButton);

	_homeScreen = new HomeScreen();
	_homeScreen->setGeometry(0, 0, 480, 224);
	addWidget(_homeScreen);

	_hawkScreen = new HawkScreen();
	_hawkScreen->setVisible(false);
	_hawkScreen->setGeometry(0, 0, 480, 224);
	addWidget(_hawkScreen);

	_statScreen = new StatScreen();
	_statScreen->setVisible(false);
	_statScreen->setGeometry(0, 0, 480, 224);
	addWidget(_statScreen);

#ifdef PXA1826
	/* init pipe */
	if (pipe(_fd))
		ILOG_THROW(MGUI_APP, "pipe failed with error\n");
	_ubus = new UBusThread(_statusBar, _bottomBar, _cellular, _wireless, _fd[0]);
	_ubus->start();
	_onkey = new OnkeyThread(_fd[1]);
	_onkey->sigOnkeyPress.connect(sigc::mem_fun(this, &MGuiApplication::MGuiStateToggle));
	_onkey->start();

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

void
MGuiApplication::switchScreen(int screen)
{
    if (screen == 0)
    {
        _homeScreen->setVisible(true);
        _hawkScreen->setVisible(false);
        _statScreen->setVisible(false);
        _homeButton->setChecked(true);
        _hawkButton->setChecked(false);
        _statButton->setChecked(false);
    } else if (screen == 1)
    {
        _homeScreen->setVisible(false);
        _hawkScreen->setVisible(true);
        _statScreen->setVisible(false);
        _homeButton->setChecked(false);
        _hawkButton->setChecked(true);
        _statButton->setChecked(false);
    } else if (screen == 2)
    {
        _homeScreen->setVisible(false);
        _hawkScreen->setVisible(false);
        _statScreen->setVisible(true);
        _homeButton->setChecked(false);
        _hawkButton->setChecked(false);
        _statButton->setChecked(true);
    }
}

} /* namespace MGUI */
