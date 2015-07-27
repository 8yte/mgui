#ifndef SRC_ONKEYTHREAD_H_
#define SRC_ONKEYTHREAD_H_

extern "C" {
#include <libubox/ustream.h>
#include <libubus.h>
}
#include <lib/Thread.h>

namespace MGUI
{

class OnkeyThread : public ilixi::Thread
{
public:
    OnkeyThread(int pipe_fd);

    virtual
    ~OnkeyThread();

    virtual int
    run();

private:
    PlatformEvent _event;
    const char* _dev;
    int _pipe;
};

} /* namespace MGUI */

#endif /* SRC_ONKEYTHREAD_H_ */
