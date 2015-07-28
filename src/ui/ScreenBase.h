#ifndef SRC_UI_SCREENBASE_H_
#define SRC_UI_SCREENBASE_H_

#include <ui/Widget.h>

#ifdef PXA1826
#include <platform/pxa1826/UBusThread.h>
#include <platform/pxa1826/OnkeyThread.h>
#include <platform/pxa1826/MGuiRil.h>
#include <platform/pxa1826/MGuiCharger.h>
#include <platform/pxa1826/MGuiWifi.h>
#include <platform/pxa1826/MGuiStats.h>
#include <platform/pxa1826/MGuiHawk.h>
#endif

namespace MGUI
{

class ScreenBase : public ilixi::Widget
{
public:
    ScreenBase(ilixi::Widget* parent = NULL);

    virtual
    ~ScreenBase();

protected:
    virtual void
    compose(const ilixi::PaintEvent& event);
};

} /* namespace MGUI */

#endif /* SRC_UI_SCREENBASE_H_ */
