#ifndef SRC_MGUI_RIL_H_
#define SRC_MGUI_RIL_H_

#include <vector>
#include "UBusClient.h"
#include "RilRequest.h"

namespace MGUI
{

class HomeScreen;

class MGuiRil : public UBusClient
{
public:
    static MGuiRil*
    Instance();

    int
    Indication(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req,
		      const char *method, struct blob_attr *msg);

    void
    SignalStrengthInd(void *data, int len);

    static int
    Create(ubus_context *ubus, StatusBar *bar, HomeScreen *home);

    static void
    Destroy();

    virtual int
    Register();

    virtual int
    UnRegister();

    void
    SendRequests();

    int
    OperatorCallback(blob_attr *msg);

    int
    SimCallback(blob_attr *msg);

    int
    RegistrationCallback(blob_attr *msg);

protected:
    MGuiRil(ubus_context *ubus, StatusBar *bar, HomeScreen *home);

    virtual
    ~MGuiRil();

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

    enum {
        REQID,
        ERRNO,
        DATA,
        _MAX
    };

    int
    parse_blobmsg(struct blob_attr *attr, struct blob_attr **tb);

    static const std::vector<std::string> _events;
    static MGuiRil* _instance;
    StatusBar* _bar;
    RilOperator* _operator;
    RilSimcard* _simcard;
    RilRegistration* _registration;
    RilScreen* _screen;

    HomeScreen* _home;
};

} /* namespace MGUI */

#endif /* SRC_MGUI_RIL_H_ */
