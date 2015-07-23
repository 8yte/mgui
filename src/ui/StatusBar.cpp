#include "StatusBar.h"
#include <ui/HBoxLayout.h>
#include <ui/Spacer.h>

namespace MGUI
{

StatusBar::StatusBar(ilixi::Widget* parent)
        : ContainerBase(parent),
          _batteryIcon(NULL)
{
    ilixi::HBoxLayout* layout = new ilixi::HBoxLayout();
    layout->setSpacing(0);
    layout->setVerticalAlignment(ilixi::Alignment::Middle);
    setLayout(layout);

    _clock = new ilixi::Label("00:00");
    addWidget(_clock);

    addWidget(new ilixi::Spacer());

    _simIcon = new SimIcon();
    addWidget(_simIcon);

    _sdcardIcon = new SDCardIcon();
    addWidget(_sdcardIcon);

    _wifiIcon = new WiFiIcon();
    addWidget(_wifiIcon);

    _cellularTechIcon = new CellularTechIcon();
    addWidget(_cellularTechIcon);

    _cellularIcon = new CellularIcon();
    addWidget(_cellularIcon);

    _gpsIcon = new GPSIcon();
    addWidget(_gpsIcon);

    _bluetoothIcon = new BTIcon();
    addWidget(_bluetoothIcon);

    _batteryIcon = new BatteryIcon();
    addWidget(_batteryIcon);

    _timer = new ilixi::Timer();
    _timer->sigExec.connect(sigc::mem_fun(this, &StatusBar::updateTime));
    _timer->start(10000);
    updateTime();
}

StatusBar::~StatusBar()
{
    delete _timer;
}

void
StatusBar::compose(const ilixi::PaintEvent& event)
{
}

void
StatusBar::setBatteryState(BatteryState state)
{
    _batteryIcon->setBatteryState(state);
}

void
StatusBar::setBluetoothState(BluetoothState state)
{
    _bluetoothIcon->setBluetoothState(state);
}

void
StatusBar::setCellularState(CellularState state)
{
    _cellularIcon->setCellularState(state);
}

void
StatusBar::setCellularTechState(CellularTechState state)
{
    _cellularTechIcon->setCellularTechState(state);
}

void
StatusBar::setGPSState(GpsState state)
{
    _gpsIcon->setGpsState(state);
}

void
StatusBar::setSDCardState(SDCardState state)
{
    _sdcardIcon->setSdcardState(state);
}

void
StatusBar::setSimState(SimState state)
{
    _simIcon->setSimState(state);
}

void
StatusBar::setWifiState(WifiState state)
{
    _wifiIcon->setWifiState(state);
}

void
StatusBar::updateTime()
{
    struct timeval tv;
    struct tm* tm;
    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);

    _clock->setText(ilixi::PrintF("%02d:%02d", tm->tm_hour, tm->tm_min));
}

} /* namespace MGUI */
