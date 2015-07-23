#ifndef SRC_MGUI_RIL_H_
#define SRC_MGUI_RIL_H_

#include <vector>
#include "UBusSubscriber.h"
#include "rilutil.h"
#include "ril.h"

namespace MGUI
{

class MGuiRil : public UBusSubscriber
{
public:
    MGuiRil(ubus_context* ubus);

    virtual
    ~MGuiRil();

    virtual int
    Register(ubus_handler_t cb, const char *name);

    virtual int
    UnRegister();

    virtual int
    Subscribe(std::string &event);

    virtual int
    UnSubscribe(std::string &event);

    virtual int
    Subscribe(std::vector<std::string> &events);

    virtual int
    UnSubscribe(std::vector<std::string> &events);

private:
    struct RilRegInfo {
        /* 0 not registered, ME is not currently searching a new operator to register to
         * 1 registered, home network
         * 2 not registered, but ME is currently searching a new operator to register to
         * 3 registration denied
         * 4 unknown
         * 5 registered, roaming */
        unsigned int reg_state;
        RIL_RadioTechnology radio_tech;
    };

private:
    RilRegInfo _voice_registration;
    RilRegInfo _data_registration;
    RIL_CardState _simcard_state;
    std::string _operator;
    unsigned int rssi;
    static const std::vector<std::string> _events;
};

} /* namespace MGUI */

#endif /* SRC_MGUI_RIL_H_ */
