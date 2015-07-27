#ifndef SRC_MGUI_WIFI_REQUEST_H_
#define SRC_MGUI_WIFI_REQUEST_H_

#include "UBusClient.h"
#include "UBusRequest.h"
#include "MGuiWifi.h"

namespace MGUI
{

class WifiRequest : public UBusRequest
{
public:
    WifiRequest(ubus_subscriber &subscriber,
               uint32_t &ubus_id, ubus_context *ubus);

    virtual
    ~WifiRequest();

    int
    Request();

    virtual int
    Callback(ubus_request *req, blob_attr *msg);

};

};

#endif /* SRC_MGUI_CHARGER_REQUEST_H_ */
