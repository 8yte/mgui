#ifndef SRC_UBUSTHREAD_H_
#define SRC_UBUSTHREAD_H_

#include <lib/Thread.h>
extern "C" {
#include <libubox/ustream.h>
#include <libubus.h>
}

namespace MGUI
{

class UBusThread : public ilixi::Thread
{
public:
    UBusThread(int pipe_fd);

    virtual
    ~UBusThread();

    virtual int
    run();

    ubus_context*
    GetContext();

private:
    ubus_context* _ubus;
    uloop_fd _ubus_fd;
};

} /* namespace MGUI */

#endif /* SRC_UBUSTHREAD_H_ */
