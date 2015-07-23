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

    uloop_init();
    _ubus = ubus_connect(NULL);
    if (!_ubus)
        ILOG_THROW(UBUS_THREAD, "Failed to connect to ubus\n");

    ubus_add_uloop(_ubus);
    MGuiRil::Create(_ubus, _bar);

    ILOG_DEBUG(UBUS_THREAD, "%s exit\n", __func__);
}

UBusThread::~UBusThread()
{
    ILOG_TRACE(UBUS_THREAD);

    ubus_free(_ubus);
    uloop_done();

    MGuiRil::Destroy();

    ILOG_DEBUG(UBUS_THREAD, "%s exit\n", __func__);
}

int
UBusThread::run()
{
    ILOG_TRACE(UBUS_THREAD);
    uloop_run();
    return 0;
}

} /* namespace MGUI */
