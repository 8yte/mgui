
#ifndef SRC_UBUS_REQUEST_H_
#define SRC_UBUS_REQUEST_H_

extern "C" {
#include <libubox/ustream.h>
#include <libubus.h>
}
#include <iostream>

namespace MGUI
{

class UBusRequest
{
public:
    UBusRequest(ubus_subscriber &subscriber, uint32_t &id, ubus_context* ubus);

    virtual
    ~UBusRequest();

    virtual int
    Callback(ubus_request *req, blob_attr *msg);

    int
    Invoke(const char *method, blob_attr *b);

    int
    InvokeAsync(const char *method, blob_attr *b);

protected:
    ubus_subscriber _subscriber;
    uint32_t _id;
    ubus_context* _ubus;
};

} /* namespace MGUI */

#endif /* SRC_UBUS_REQUEST_H_ */
