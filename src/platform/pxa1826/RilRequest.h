#ifndef SRC_MGUI_RIL_REQUEST_H_
#define SRC_MGUI_RIL_REQUEST_H_

#include <MGuiPlatform.h>
#include "UBusClient.h"
#include "UBusRequest.h"
#include "rilutil.h"
#include "ril.h"

namespace MGUI
{


class RilRequest : public UBusRequest
{
public:
    RilRequest(ubus_subscriber &subscriber,
               uint32_t &ubus_id, ubus_context *ubus);

    virtual
    ~RilRequest();

    virtual int
    Request(uint32_t id);

    virtual int
    Request(uint32_t id, const void *data, int len);

    virtual int
    Callback(ubus_request *req, blob_attr *msg) = 0;

protected:
    blob_buf _b;
};

class RilOperator : public RilRequest
{
public:
    RilOperator(ubus_subscriber &subscriber,
                uint32_t &ubus_id, ubus_context *ubus);

    virtual
    ~RilOperator();

    virtual int
    Request();

    virtual int
    Callback(ubus_request *req, blob_attr *msg);
};

class RilSimcard : public RilRequest
{
public:
    RilSimcard(ubus_subscriber &subscriber,
               uint32_t &ubus_id, ubus_context *ubus);

    virtual
    ~RilSimcard();

    virtual int
    Request();

    virtual int
    Callback(ubus_request *req, blob_attr *msg);
};

class RilScreen : public RilRequest
{
public:
    RilScreen(ubus_subscriber &subscriber,
              uint32_t &ubus_id, ubus_context *ubus);

    virtual
    ~RilScreen();

    int
    Request(int on_off);

    virtual int
    Callback(ubus_request *req, blob_attr *msg);
};

class RilRegistration : public RilRequest
{
public:
    RilRegistration(ubus_subscriber &subscriber,
                    uint32_t &ubus_id, ubus_context *ubus);

    virtual
    ~RilRegistration();

    virtual
    int Request();

    virtual int
    Callback(ubus_request *req, blob_attr *msg);

};

};

#endif /* SRC_MGUI_RIL_REQUEST_H_ */
