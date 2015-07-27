#include "BottomBar.h"
#include <ui/HBoxLayout.h>

namespace MGUI
{

BottomBar::BottomBar(ilixi::Widget* parent)
        : ContainerBase(parent)
{
    ilixi::HBoxLayout* layout = new ilixi::HBoxLayout();
    layout->setVerticalAlignment(ilixi::Alignment::Middle);
    setLayout(layout);
}

BottomBar::~BottomBar()
{
}

void
BottomBar::compose(const ilixi::PaintEvent& event)
{
}

void
BottomBar::SetVersion(const std::string &version)
{	
    _version = new ilixi::Label(version);
    _version->setLayoutAlignment(ilixi::TextLayout::Center);
    addWidget(_version);
}

} /* namespace MGUI */
