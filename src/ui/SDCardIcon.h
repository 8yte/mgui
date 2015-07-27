#ifndef SDCARDICON_H_
#define SDCARDICON_H_

#include "SimpleIcon.h"
#ifdef PXA1826
extern "C" {
#include <libgen.h>
#include <dirent.h>
#include <uci.h>
#include <uci_blob.h>
}
#endif

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
    int
    UciDetectSdcard(char *cfg);

    SDCardState _sdcardState;
};

} /* namespace MGUI */

#endif /* SDCARDICON_H_ */
