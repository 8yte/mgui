#ifndef SRC_UBUSTHREAD_H_
#define SRC_UBUSTHREAD_H_

#include <lib/Thread.h>
extern "C" {
#include <libubox/ustream.h>
#include <libubus.h>
}
#include <cutils/properties.h>
#include "ril.h"
#include "rilutil.h"
#include "StatusBar.h"

namespace MGUI
{

class UBusThread : public ilixi::Thread
{
public:
    UBusThread(StatusBar* target);

    virtual
    ~UBusThread();

    virtual int
    run();

private:
    StatusBar* _bar;
    ubus_context* _ubus;
};

} /* namespace MGUI */

#endif /* SRC_UBUSTHREAD_H_ */
