#ifndef SRC_UBUSSUBSCRIBER_H_
#define SRC_UBUSSUBSCRIBER_H_

extern "C" {
#include <libubox/ustream.h>
#include <libubus.h>
}
#include <iostream>
#include <vector>

namespace MGUI
{

class UBusSubscriber
{
public:
    UBusSubscriber(ubus_context* ubus);

    virtual
    ~UBusSubscriber();

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
    ubus_subscriber _subscriber;
    uint32_t _id;
    ubus_context* _ubus;
};

} /* namespace MGUI */

#endif /* SRC_UBUSSUBSCRIBER_H_ */
