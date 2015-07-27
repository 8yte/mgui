#ifndef SRC_UBUS_CLIENT_H_
#define SRC_UBUS_CLIENT_H_

extern "C" {
#include <libubox/ustream.h>
#include <libubus.h>
}
#include <iostream>
#include <vector>

namespace MGUI
{

class UBusClient
{
public:
    UBusClient(ubus_context* ubus);

    virtual
    ~UBusClient();

    virtual int
    Register() = 0;

    virtual int
    UnRegister();

protected:
    virtual int
    Register(ubus_handler_t cb, const char *name);

    virtual int
    Subscribe();

    virtual int
    UnSubscribe();

    virtual int
    Subscribe(const std::string &event);

    virtual int
    UnSubscribe(const std::string &event);

    virtual int
    Subscribe(const std::vector<std::string> &events);

    virtual int
    UnSubscribe(const std::vector<std::string> &events);

    ubus_subscriber _subscriber;
    uint32_t _id;
    ubus_context* _ubus;
};

} /* namespace MGUI */

#endif /* SRC_UBUS_CLIENT_H_ */
