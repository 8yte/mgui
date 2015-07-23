#ifndef SIMICON_H_
#define SIMICON_H_

#include "SimpleIcon.h"

namespace MGUI
{

class SimIcon: public SimpleIcon
{
public:
    SimIcon(ilixi::Widget* parent = NULL);

    virtual
    ~SimIcon();

    SimState
    getSimState() const;

    void
    setSimState(SimState simState);

private:
    SimState _simState;
};

} /* namespace MGUI */

#endif /* SIMICON_H_ */
