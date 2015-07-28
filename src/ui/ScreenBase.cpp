#include "ScreenBase.h"

namespace MGUI
{

ScreenBase::ScreenBase(ilixi::Widget* parent)
        : Widget(parent)
{
    setInputMethod(ilixi::PointerInput);
}

ScreenBase::~ScreenBase()
{
}

void
ScreenBase::compose(const ilixi::PaintEvent& event)
{
}

} /* namespace MGUI */
