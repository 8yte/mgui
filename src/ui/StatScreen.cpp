#include "StatScreen.h"

namespace MGUI
{

StatScreen::StatScreen()
        : ScreenBase()
{
    _title = new ilixi::Label("Statistics");
    _title->setGeometry(0, 0, 480, 50);
    addChild(_title);
}

StatScreen::~StatScreen()
{
}

} /* namespace MGUI */
