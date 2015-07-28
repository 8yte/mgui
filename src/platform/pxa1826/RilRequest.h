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

    virtual void
    Indication(uint32_t &id, void *data, int len);

protected:
    blob_buf _b;
};

class RilOperator : public RilRequest
{
public:
    RilOperator(ubus_subscriber &subscriber, uint32_t &ubus_id, ubus_context *ubus, StatusBar *bar, ilixi::ToolButton* _button);

    virtual
    ~RilOperator();

    virtual int
    Request();

    virtual int
    Callback(ubus_request *req, blob_attr *msg);

private:
    StatusBar* _bar;
    ilixi::ToolButton* _button;
};

class RilSimcard : public RilRequest
{
public:
    RilSimcard(ubus_subscriber &subscriber, uint32_t &ubus_id, ubus_context *ubus, StatusBar *bar);

    virtual
    ~RilSimcard();

    virtual int
    Request();

    virtual int
    Callback(ubus_request *req, blob_attr *msg);

private:
    StatusBar* _bar;
};

class RilScreen : public RilRequest
{
public:
    RilScreen(ubus_subscriber &subscriber, uint32_t &ubus_id, ubus_context *ubus);

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
    RilRegistration(ubus_subscriber &subscriber, uint32_t &ubus_id, ubus_context *ubus, StatusBar *bar);

    virtual
    ~RilRegistration();

    virtual
    int Request();

    virtual int
    Callback(ubus_request *req, blob_attr *msg);

    virtual void
    Indication(uint32_t &id, void *data, int len);

private:
    struct RegState {
        unsigned int reg_state;
        RIL_RadioTechnology radio_tech;
    };

    bool
    registered_voice();

    bool
    registered_data();

    CellularTechState
    tech();

    const char *
    tech_to_str();

    int
    convertDbmToRssi(int rsrp);

    int _rssi;
    RegState _data;
    RegState _voice;
    StatusBar* _bar;
};

};

#endif /* SRC_MGUI_RIL_REQUEST_H_ */
