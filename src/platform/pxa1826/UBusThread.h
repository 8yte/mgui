#ifndef SRC_UBUSTHREAD_H_
#define SRC_UBUSTHREAD_H_

#include <lib/Thread.h>
#include <ui/StatusBar.h>
extern "C" {
#include <libubox/ustream.h>
#include <libubus.h>
}
#include "ril.h"
#include "rilutil.h"
#include "MGuiRil.h"
#include "MGuiCharger.h"

namespace MGUI
{

class UBusThread : public ilixi::Thread
{
public:
    UBusThread(StatusBar* target, int pipe_fd);

    virtual
    ~UBusThread();

    virtual int
    run();

private:
    StatusBar* _bar;
    ubus_context* _ubus;
    uloop_fd _ubus_fd;
};

} /* namespace MGUI */

#endif /* SRC_UBUSTHREAD_H_ */
