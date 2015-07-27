#ifndef SRC_ONKEYTHREAD_H_
#define SRC_ONKEYTHREAD_H_

extern "C" {
#include <libubox/ustream.h>
#include <libubus.h>
}
#include <lib/Thread.h>

namespace MGUI
{

class OnkeyThread : public ilixi::Thread
{
public:
    OnkeyThread(ubus_context *ubus);

    virtual
    ~OnkeyThread();

    virtual int
    run();

private:
    ubus_context* _ubus;
};

} /* namespace MGUI */

#endif /* SRC_ONKEYTHREAD_H_ */
