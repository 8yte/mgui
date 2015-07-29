#include "HomeScreen.h"

#include <ui/PushButton.h>
#include <ui/ToolButton.h>
#include <ui/Dialog.h>

namespace MGUI
{

HomeScreen::HomeScreen()
        : ScreenBase()
{
	_cellular = new ilixi::ToolButton("Cellular");
	_cellular->setGeometry(315, 28, 160, 80);
	//_cellular->setIcon(__iconPack->getIcon("cellular_4_bar")->getImagePath());
	addChild(_cellular);

	_wireless = new ilixi::ToolButton("Wireless");
	//_wireless->setIcon(__iconPack->getIcon("wifi_4_bar")->getImagePath());
	_wireless->setGeometry(315, 118, 160, 80);
	addChild(_wireless);
}

HomeScreen::~HomeScreen()
{
}

} /* namespace MGUI */
