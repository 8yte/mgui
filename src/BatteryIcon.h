#ifndef BATTERYICON_H_
#define BATTERYICON_H_

#include "SimpleIcon.h"
#include "Defines.h"

namespace MGUI
{

class BatteryIcon : public SimpleIcon
{
public:
    BatteryIcon(Widget* parent = NULL);

    virtual
    ~BatteryIcon();

    BATTERY_STATES
    getBatteryState() const;

    void
    setBatteryState(BATTERY_STATES iconState);

private:
    BATTERY_STATES _batteryState;
};

} /* namespace MGUI */

#endif /* BATTERYICON_H_ */
