#ifndef GPSICON_H_
#define GPSICON_H_

#include "SimpleIcon.h"

namespace MGUI
{

class GPSIcon: public SimpleIcon
{
public:
    GPSIcon(ilixi::Widget* parent = NULL);

    virtual
    ~GPSIcon();

    GpsState
    getGpsState() const;

    void
    setGpsState(GpsState gpsState);

private:
    GpsState _gpsState;
};

} /* namespace MGUI */

#endif /* GPSICON_H_ */
