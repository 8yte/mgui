#include "UBusThread.h"

namespace MGUI
{

UBusThread::UBusThread(StatusBar* bar)
        : Thread(),
          _bar(bar)
{
}

UBusThread::~UBusThread()
{
}

int
UBusThread::run()
{
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
}

} /* namespace MGUI */
