#include "SDCardIcon.h"

namespace MGUI
{

SDCardIcon::SDCardIcon(ilixi::Widget* parent)
        : SimpleIcon(parent),
          _sdcardState(SDCardUnknown)
{
    setSdcardState(SDCardMissing);
}

SDCardIcon::~SDCardIcon()
{
}

SDCardState
SDCardIcon::getSdcardState() const
{
    return _sdcardState;
}

void
SDCardIcon::setSdcardState(SDCardState sdcardState)
{
    if (_sdcardState != sdcardState)
    {
        _sdcardState = sdcardState;
        switch (_sdcardState)
        {
            case SDCardActive:
                setImage("sdcard");
                break;
            case SDCardMissing:
                setImage("sdcard_missing");
                break;
            default:
                break;
        }
    }
}

} /* namespace MGUI */
