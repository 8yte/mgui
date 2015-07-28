#ifndef SRC_UI_HAWKSCREEN_H_
#define SRC_UI_HAWKSCREEN_H_

#include "ScreenBase.h"

#include <ui/Label.h>

namespace MGUI
{

class HawkScreen : public ScreenBase
{
public:
    HawkScreen();

    virtual
    ~HawkScreen();

private:
    ilixi::Label* _title;
};

} /* namespace MGUI */

#endif /* SRC_UI_HAWKSCREEN_H_ */
