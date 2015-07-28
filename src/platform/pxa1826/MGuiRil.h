#ifndef SRC_MGUI_RIL_H_
#define SRC_MGUI_RIL_H_

#include <vector>
#include "UBusClient.h"
#include "RilRequest.h"

namespace MGUI
{

class MGuiRil : public UBusClient
{
public:
    static MGuiRil*
    Instance();

    int
    Indication(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req,
		      const char *method, struct blob_attr *msg);

    static int
    Create(ubus_context *ubus, StatusBar *bar, ilixi::ToolButton* button);

    static void
    Destroy();

    virtual int
    Register();

    virtual int
    UnRegister();

    void
    SendRequests();

protected:
    MGuiRil(ubus_context* ubus, StatusBar* bar, ilixi::ToolButton* button);

    virtual
    ~MGuiRil();

private:
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
    ilixi::ToolButton* _button;
};

} /* namespace MGUI */

#endif /* SRC_MGUI_RIL_H_ */
