#ifndef SRC_MGUI_STATS_REQUEST_H_
#define SRC_MGUI_STATS_REQUEST_H_

#include "UBusClient.h"
#include "UBusRequest.h"
#include "MGuiStats.h"

namespace MGUI
{

class StatsRequest : public UBusRequest
{
public:
    StatsRequest(ubus_subscriber &subscriber,
               uint32_t &ubus_id, ubus_context *ubus);

    virtual
    ~StatsRequest();

    int
    Request();

    virtual int
    Callback(ubus_request *req, blob_attr *msg);

};

};

#endif /* SRC_MGUI_STATS_REQUEST_H_ */
