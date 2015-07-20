#include "MGuiApplication.h"
#include <ui/PushButton.h>
#include <ui/ToolButton.h>

namespace MGUI
{

MGuiApplication::MGuiApplication(int argc, char* argv[])
        : Application(&argc, &argv)
{
    _statusBar = new StatusBar();
    _statusBar->setGeometry(5, 0, 470, 24);
    addWidget(_statusBar);

    _bottomBar = new BottomBar();
    _bottomBar->setGeometry(0, 248, 480, 24);
    addWidget(_bottomBar);

    _resetButton = new ilixi::PushButton("Reset");
    _resetButton->setGeometry(5, 48, 160, 40);
    addWidget(_resetButton);

    _fotaButton = new ilixi::PushButton("Fota");
    _fotaButton->setGeometry(5, 93, 160, 40);
    addWidget(_fotaButton);

    _assertButton = new ilixi::PushButton("No data assert");
    _assertButton->setGeometry(5, 138, 160, 40);
    addWidget(_assertButton);

    _keepAliveButton = new ilixi::PushButton("Keep alive");
    _keepAliveButton->setGeometry(5, 183, 160, 40);
    addWidget(_keepAliveButton);

    _cellular = new ilixi::ToolButton("Cellular");
    _cellular->setGeometry(315, 48, 160, 80);
    addWidget(_cellular);

    _wireless = new ilixi::ToolButton("Wireless");
    _wireless->setGeometry(315, 138, 160, 80);
    addWidget(_wireless);

    _thread = new UBusThread(_statusBar);
    _thread->start();
}

MGuiApplication::~MGuiApplication()
{
    _thread->cancel();
    delete _thread;
}

} /* namespace MGUI */
