#ifndef SRC_UI_STATSCREEN_H_
#define SRC_UI_STATSCREEN_H_

#include "ScreenBase.h"

#include <ui/Label.h>

namespace MGUI
{

class StatScreen : public ScreenBase
{
public:
    StatScreen();

    virtual
    ~StatScreen();

private:
    ilixi::Label* _title;
};

} /* namespace MGUI */

#endif /* SRC_UI_STATSCREEN_H_ */
