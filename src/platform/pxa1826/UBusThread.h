#ifndef SRC_UBUSTHREAD_H_
#define SRC_UBUSTHREAD_H_

#include <lib/Thread.h>
#include <ui/ToolButton.h>
#include <ui/StatusBar.h>
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

class HomeScreen;
class HawkScreen;
class StatScreen;
class StatusBar;

class UBusThread : public ilixi::Thread
{
public:
    UBusThread(StatusBar* bar, HomeScreen* home, HawkScreen* hawk, StatScreen* stat, int fd);

    virtual
    ~UBusThread();

    virtual int
    run();

private:
    StatusBar* _bar;
    ubus_context* _ubus;
    uloop_fd _ubus_fd;
    HomeScreen* _home;
    HawkScreen* _hawk;
    StatScreen* _stat;
};

} /* namespace MGUI */

#endif /* SRC_UBUSTHREAD_H_ */
