#include <core/Logger.h>
#include <MGuiPlatform.h>
#include "UBusThread.h"

namespace MGUI
{

D_DEBUG_DOMAIN(UBUS_THREAD, "Ubus/Thread", "Thread");

static void UBusThreadEvent(struct uloop_fd *u, unsigned int events)
{
    PlatformEvent event;
    read(u->fd, &event, sizeof(event));
    std::cout << "UBusThreadEvent: " << event <<std::endl;
}

UBusThread::UBusThread(StatusBar* bar, int fd)
        : Thread(),
          _bar(bar)
{
    ILOG_TRACE(UBUS_THREAD);

    uloop_init();
    _ubus = ubus_connect(NULL);
    if (!_ubus)
        ILOG_THROW(UBUS_THREAD, "Failed to connect to ubus\n");

    ubus_add_uloop(_ubus);
    _ubus_fd.cb = UBusThreadEvent;
    _ubus_fd.fd = fd;

    MGuiRil::Create(_ubus, _bar);
    MGuiCharger::Create(_ubus, _bar);

    ILOG_DEBUG(UBUS_THREAD, "%s exit\n", __func__);
}

UBusThread::~UBusThread()
{
    ILOG_TRACE(UBUS_THREAD);

    ubus_free(_ubus);
    uloop_done();

    MGuiRil::Destroy();
    MGuiCharger::Destroy();

    ILOG_DEBUG(UBUS_THREAD, "%s exit\n", __func__);
}

int
UBusThread::run()
{
    ILOG_TRACE(UBUS_THREAD);
    uloop_fd_add(&_ubus_fd, ULOOP_READ);
    uloop_run();
    return 0;
}

} /* namespace MGUI */
