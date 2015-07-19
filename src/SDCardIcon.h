#ifndef SDCARDICON_H_
#define SDCARDICON_H_

#include "SimpleIcon.h"

namespace MGUI
{

class SDCardIcon : public SimpleIcon
{
public:
    SDCardIcon(ilixi::Widget* parent = NULL);

    virtual
    ~SDCardIcon();

    SDCardState
    getSdcardState() const;

    void
    setSdcardState(SDCardState sdcardState);

private:
    SDCardState _sdcardState;
};

} /* namespace MGUI */

#endif /* SDCARDICON_H_ */
