#ifndef SRC_UBUSTHREAD_H_
#define SRC_UBUSTHREAD_H_

#include <lib/Thread.h>
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
};

} /* namespace MGUI */

#endif /* SRC_UBUSTHREAD_H_ */
