#include "StatusBar.h"
#include <ui/HBoxLayout.h>
#include <ui/Spacer.h>

namespace MGUI
{

StatusBar::StatusBar(ilixi::Widget* parent)
        : ContainerBase(parent),
          _batteryIcon(NULL)
{
    ilixi::HBoxLayout* layout = new ilixi::HBoxLayout();
    layout->setVerticalAlignment(ilixi::Alignment::Middle);
    setLayout(layout);

    _clock = new ilixi::Label("00:00");
    addWidget(_clock);

    addWidget(new ilixi::Spacer());

    _batteryIcon = new BatteryIcon();
    addWidget(_batteryIcon);

    _timer = new ilixi::Timer();
    _timer->sigExec.connect(sigc::mem_fun(this, &StatusBar::updateTime));
    _timer->start(10000);
    updateTime();
}

StatusBar::~StatusBar()
{
    delete _timer;
}

void
StatusBar::compose(const ilixi::PaintEvent& event)
{
}

void
StatusBar::updateTime()
{
    struct timeval tv;
    struct tm* tm;
    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);

    _clock->setText(ilixi::PrintF("%02d:%02d", tm->tm_hour, tm->tm_min));
}

} /* namespace MGUI */
