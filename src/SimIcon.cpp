#include "SimIcon.h"

namespace MGUI
{

SimIcon::SimIcon(ilixi::Widget* parent)
        : SimpleIcon(parent),
          _simState(SimUnknown)
{
    setSimState(SimMissing);
}

SimIcon::~SimIcon()
{
}

SimState
SimIcon::getSimState() const
{
    return _simState;
}

void
SimIcon::setSimState(SimState simState)
{
    if (_simState != simState)
    {
        _simState = simState;
        switch (_simState)
        {
            case SimActive:
                setImage("simcard");
                break;
            case SimMissing:
                setImage("simcard_missing");
                break;
            default:
                break;
        }
    }
}

} /* namespace MGUI */
