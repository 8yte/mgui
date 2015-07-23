#include <core/Logger.h>
#include "UBusThread.h"

namespace MGUI
{

D_DEBUG_DOMAIN(UBUS_THREAD, "Ubus/Thread", "Thread");

UBusThread::UBusThread(StatusBar* bar)
        : Thread(),
          _bar(bar)
{
    ILOG_TRACE(UBUS_THREAD);
#ifdef HAVE_UBUS
    uloop_init();

    _ubus = ubus_connect(NULL);
    if (!_ubus)
        ILOG_THROW(UBUS_THREAD, "Failed to connect to ubus\n");

    ubus_add_uloop(_ubus);
#endif /* HAVE_UBUS */
    ILOG_DEBUG(UBUS_THREAD, "%s exit\n", __func__);
}

UBusThread::~UBusThread()
{
    ILOG_TRACE(UBUS_THREAD);
#ifdef HAVE_UBUS
    ubus_free(_ubus);
    uloop_done();
#endif /* HAVE_UBUS */
    ILOG_DEBUG(UBUS_THREAD, "%s exit\n", __func__);
}

int
UBusThread::run()
{
    ILOG_TRACE(UBUS_THREAD);
#ifdef HAVE_UBUS
    uloop_run();
    return 1;
#else
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
#endif /* HAVE_UBUS */
}

} /* namespace MGUI */
