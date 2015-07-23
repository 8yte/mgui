#include "BatteryIcon.h"

namespace MGUI
{

BatteryIcon::BatteryIcon(Widget* parent)
        : SimpleIcon(parent),
          _batteryState(BatteryAlert)
{
    setBatteryState(BatteryUnknown);
}

BatteryIcon::~BatteryIcon()
{
}

BatteryState
BatteryIcon::getBatteryState() const
{
    return _batteryState;
}

void
BatteryIcon::setBatteryState(BatteryState batteryState)
{
    if (_batteryState != batteryState)
    {
        _batteryState = batteryState;
        switch (batteryState)
        {
            case BatteryCharging20:
                setImage("battery_20_charging");
                break;
            case BatteryCharging30:
                setImage("battery_30_charging");
                break;
            case BatteryCharging50:
                setImage("battery_50_charging");
                break;
            case BatteryCharging60:
                setImage("battery_60_charging");
                break;
            case BatteryCharging80:
                setImage("battery_80_charging");
                break;
            case BatteryCharging90:
                setImage("battery_90_charging");
                break;
            case BatteryCharging100:
                setImage("battery_100_charging");
                break;
            case Battery20:
                setImage("battery_20");
                break;
            case Battery30:
                setImage("battery_30");
                break;
            case Battery50:
                setImage("battery_50");
                break;
            case Battery60:
                setImage("battery_60");
                break;
            case Battery80:
                setImage("battery_80");
                break;
            case Battery90:
                setImage("battery_90");
                break;
            case Battery100:
                setImage("battery_100");
                break;
            case BatteryAlert:
                setImage("battery_alert");
                break;
            default:
                setImage("battery_unknown");
                break;
        }
    }
}

} /* namespace MGUI */
