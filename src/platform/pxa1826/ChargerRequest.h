#ifndef SRC_MGUI_CHARGER_REQUEST_H_
#define SRC_MGUI_CHARGER_REQUEST_H_

#include <MGuiPlatform.h>
#include "UBusClient.h"
#include "UBusRequest.h"
#include "MGuiCharger.h"

namespace MGUI
{

class ChargerRequest : public UBusRequest
{
public:
    ChargerRequest(ubus_subscriber &subscriber,
               uint32_t &ubus_id, ubus_context *ubus);

    virtual
    ~ChargerRequest();

    int
    Request();

    virtual int
    Callback(ubus_request *req, blob_attr *msg);

};

};

#endif /* SRC_MGUI_CHARGER_REQUEST_H_ */
