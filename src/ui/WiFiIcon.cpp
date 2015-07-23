#include "WiFiIcon.h"

namespace MGUI
{

WiFiIcon::WiFiIcon(ilixi::Widget* parent)
        : SimpleIcon(parent),
          _wifiState(WifiUnknown)
{
    setWifiState(WifiOff);
}

WiFiIcon::~WiFiIcon()
{
}

WifiState
WiFiIcon::getWifiState() const
{
    return _wifiState;
}

void
WiFiIcon::setWifiState(WifiState wifiState)
{
    if (_wifiState != wifiState)
    {
        _wifiState = wifiState;
        switch (_wifiState)
        {
            case WifiOff:
                setImage("wifi_off");
                break;
            case WifiTethering:
                setImage("wifi_tethering");
                break;
            case Wifi0Bar:
                setImage("wifi_0_bar");
                break;
            case Wifi1Bar:
                setImage("wifi_1_bar");
                break;
            case Wifi1BarLock:
                setImage("wifi_1_bar_lock");
                break;
            case Wifi2Bar:
                setImage("wifi_2_bar");
                break;
            case Wifi2BarLock:
                setImage("wifi_2_bar_lock");
                break;
            case Wifi3Bar:
                setImage("wifi_3_bar");
                break;
            case Wifi3BarLock:
                setImage("wifi_3_bar_lock");
                break;
            case Wifi4Bar:
                setImage("wifi_4_bar");
                break;
            case Wifi4BarLock:
                setImage("wifi_4_bar_lock");
                break;
            default:
                break;
        }
    }
}

} /* namespace MGUI */
