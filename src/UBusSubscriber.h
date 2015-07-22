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
    UBusSubscriber(ubus_context* ubus, ubus_handler_t cb);

    virtual
    ~UBusSubscriber();

    int
    Subscribe(std::string &event);

    int
    UnSubscribe(std::string &event);

    int
    Subscribe(std::vector<std::string> events);

    int
    UnSubscribe(std::vector<std::string> events);

private:
    ubus_subscriber _subscriber;
    ubus_context* _ubus;
};

} /* namespace MGUI */

#endif /* SRC_UBUSSUBSCRIBER_H_ */
