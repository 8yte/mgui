#ifndef STATUSBAR_H_
#define STATUSBAR_H_

#include <ui/ContainerBase.h>
#include <ui/Label.h>
#include <lib/Timer.h>

#include "BatteryIcon.h"
#include "BTIcon.h"
#include "CellularIcon.h"
#include "GPSIcon.h"
#include "SDCardIcon.h"
#include "SimIcon.h"
#include "WiFiIcon.h"

namespace MGUI
{

class StatusBar : public ilixi::ContainerBase
{
public:
    StatusBar(ilixi::Widget* parent = NULL);

    virtual
    ~StatusBar();

    void
    setBatteryState();

    void
    setBluetoothState();

protected:
    virtual void
    compose(const ilixi::PaintEvent& event);

    void
    updateTime();

private:
    ilixi::Label* _clock;
    ilixi::Timer* _timer;

    BatteryIcon* _batteryIcon;
    BTIcon* _bluetoothIcon;
    CellularIcon* _cellularIcon;
    GPSIcon* _gpsIcon;
    SDCardIcon* _sdcardIcon;
    SimIcon* _simIcon;
    WiFiIcon* _wifiIcon;
};

} /* namespace MGUI */

#endif /* STATUSBAR_H_ */



