#include <core/Logger.h>
#include "UBusThread.h"

namespace MGUI
{

D_DEBUG_DOMAIN(UBUS_THREAD, "Ubus Thread", "Thread");

UBusThread::UBusThread(StatusBar* bar)
        : Thread(),
          _bar(bar)
{
    ILOG_TRACE(UBUS_THREAD);
    uloop_init();

    _ubus = ubus_connect(NULL);
    if (!_ubus)
        ILOG_THROW(UBUS_THREAD, "Failed to connect to ubus\n");

    ubus_add_uloop(_ubus);
    ILOG_DEBUG(UBUS_THREAD, "%s exit\n", __func__);
}

UBusThread::~UBusThread()
{
    ILOG_TRACE(UBUS_THREAD);
    ubus_free(_ubus);
    uloop_done();
    ILOG_DEBUG(UBUS_THREAD, "%s exit\n", __func__);
}

int
UBusThread::run()
{
    ILOG_TRACE(UBUS_THREAD);
    uloop_run();
    return 1;
#if 0
    // init ubus context
    // uloop_run()...
    // not sure how to join or test for cancel thread here?
    int i = 0;
    while (true)
    {
        pthread_testcancel();
        if (i < 15)
            _bar->setBatteryState((BatteryState) i);
        else
            i = 0;
        i++;
        sleep(1);
    }
    return 1;
#endif
}

} /* namespace MGUI */
