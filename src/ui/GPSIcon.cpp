#include "GPSIcon.h"

namespace MGUI
{

GPSIcon::GPSIcon(ilixi::Widget* parent)
        : SimpleIcon(parent),
          _gpsState(GpsSearching)
{
    setGpsState(GpsOff);
}

GPSIcon::~GPSIcon()
{
}

GpsState
GPSIcon::getGpsState() const
{
    return _gpsState;
}

void
GPSIcon::setGpsState(GpsState gpsState)
{
    if (_gpsState != gpsState)
    {
        _gpsState = gpsState;
        switch (_gpsState)
        {
            case GpsOff:
                setImage("gps_off");
                break;
            case GpsSearching:
                setImage("gps_on");
                break;
            case GpsConnected:
                setImage("gps_fixed");
                break;
            default:
                break;
        }
    }
}

} /* namespace MGUI */
