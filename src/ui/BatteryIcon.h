#ifndef BATTERYICON_H_
#define BATTERYICON_H_

#include "SimpleIcon.h"

namespace MGUI
{

class BatteryIcon : public SimpleIcon
{
public:
    BatteryIcon(Widget* parent = NULL);

    virtual
    ~BatteryIcon();

    BatteryState
    getBatteryState() const;

    void
    setBatteryState(BatteryState iconState);

private:
    BatteryState _batteryState;
};

} /* namespace MGUI */

#endif /* BATTERYICON_H_ */

