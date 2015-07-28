#include "HawkScreen.h"

namespace MGUI
{

HawkScreen::HawkScreen()
        : ScreenBase()
{
    _title = new ilixi::Label("Hawk");
    _title->setGeometry(0, 0, 480, 50);
    addChild(_title);
}

HawkScreen::~HawkScreen()
{
}

} /* namespace MGUI */
