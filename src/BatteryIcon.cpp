/*
 * BatteryIcon.cpp
 *
 *  Created on: Jul 17, 2015
 *      Author: tarik
 */

#include "BatteryIcon.h"

namespace MGUI
{

BatteryIcon::BatteryIcon(Widget* parent)
        : SimpleIcon(parent),
          _batteryState(BATTERY_NONE)
{
    setBatteryState(BATTERY_CHARGING_10);
}

BatteryIcon::~BatteryIcon()
{
}

BATTERY_STATES
BatteryIcon::getBatteryState() const
{
    return _batteryState;
}

void
BatteryIcon::setBatteryState(BATTERY_STATES batteryState)
{
    if (_batteryState != batteryState)
    {
        _batteryState = batteryState;
        switch (batteryState)
        {
            case BATTERY_CHARGING_10:
                setImage("charging10");
                break;
            case BATTERY_CHARGING_20:
                setImage("charging20");
                break;
            case BATTERY_CHARGING_30:
                setImage("charging30");
                break;
            case BATTERY_CHARGING_40:
                setImage("charging40");
                break;
            case BATTERY_CHARGING_50:
                setImage("charging50");
                break;
            case BATTERY_CHARGING_60:
                setImage("charging60");
                break;
            case BATTERY_CHARGING_70:
                setImage("charging70");
                break;
            case BATTERY_CHARGING_80:
                setImage("charging80");
                break;
            case BATTERY_CHARGING_90:
                setImage("charging90");
                break;
            case BATTERY_CHARGING_100:
                setImage("charging100");
                break;
            case BATTERY_DISCHARGING_10:
                setImage("discharging10");
                break;
            case BATTERY_DISCHARGING_20:
                setImage("discharging20");
                break;
            case BATTERY_DISCHARGING_30:
                setImage("discharging30");
                break;
            case BATTERY_DISCHARGING_40:
                setImage("discharging40");
                break;
            case BATTERY_DISCHARGING_50:
                setImage("discharging50");
                break;
            case BATTERY_DISCHARGING_60:
                setImage("discharging60");
                break;
            case BATTERY_DISCHARGING_70:
                setImage("discharging70");
                break;
            case BATTERY_DISCHARGING_80:
                setImage("discharging80");
                break;
            case BATTERY_DISCHARGING_90:
                setImage("discharging90");
                break;
            case BATTERY_DISCHARGING_100:
                setImage("discharging100");
                break;
            default:
                break;
        }
    }
}

} /* namespace MGUI */
