#ifndef WIFIICON_H_
#define WIFIICON_H_

#include "SimpleIcon.h"

namespace MGUI
{

class WiFiIcon: public SimpleIcon
{
public:
    WiFiIcon(ilixi::Widget* parent = NULL);

    virtual
    ~WiFiIcon();

    WifiState
    getWifiState() const;

    void
    setWifiState(WifiState wifiState);

private:
    WifiState _wifiState;
};

} /* namespace MGUI */

#endif /* WIFIICON_H_ */
