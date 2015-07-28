#ifndef SRC_UBUSTHREAD_H_
#define SRC_UBUSTHREAD_H_

#include <lib/Thread.h>
#include <ui/StatusBar.h>
#include <ui/BottomBar.h>
#include <ui/ToolButton.h>
extern "C" {
#include <libubox/ustream.h>
#include <libubus.h>
}
#include "ril.h"
#include "rilutil.h"
#include "MGuiRil.h"
#include "MGuiCharger.h"
#include "MGuiWifi.h"
#include "MGuiStats.h"
#include "MGuiHawk.h"

namespace MGUI
{

class UBusThread : public ilixi::Thread
{
public:
    UBusThread(StatusBar* top, BottomBar *bottom , ilixi::ToolButton* cellular, ilixi::ToolButton* wireless, int fd);

    virtual
    ~UBusThread();

    virtual int
    run();

private:
    StatusBar* _top;
    BottomBar* _bottom;
    ubus_context* _ubus;
    uloop_fd _ubus_fd;
    ilixi::ToolButton* _cellular;
    ilixi::ToolButton* _wireless;
};

} /* namespace MGUI */

#endif /* SRC_UBUSTHREAD_H_ */
