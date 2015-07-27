#include <core/Logger.h>
#include <MGuiPlatform.h>
#include <iostream>
#include "UBusThread.h"

namespace MGUI
{

D_DEBUG_DOMAIN(UBUS_THREAD, "Ubus/Thread", "Thread");

static void UBusThreadEvent(struct uloop_fd *u, unsigned int events)
{
    PlatformEvent event;
    read(u->fd, &event, sizeof(event));
    std::cout << "UBusThreadEvent: " << event << std::endl;
    //TODO - put ril to sleep
}

UBusThread::UBusThread(int fd)
        : Thread()
{
    ILOG_TRACE(UBUS_THREAD);

    uloop_init();
    _ubus = ubus_connect(NULL);
    if (!_ubus)
        ILOG_THROW(UBUS_THREAD, "Failed to connect to ubus\n");

    ubus_add_uloop(_ubus);
    _ubus_fd.cb = UBusThreadEvent;
    _ubus_fd.fd = fd;

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
    uloop_fd_add(&_ubus_fd, ULOOP_READ);
    uloop_run();
    return 0;
}

ubus_context*
UBusThread::GetContext()
{
    return _ubus;
}

} /* namespace MGUI */
