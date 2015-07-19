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

    _version = new ilixi::Label("version v0.1");
    _version->setLayoutAlignment(ilixi::TextLayout::Center);
    addWidget(_version);
}

BottomBar::~BottomBar()
{
}

void
BottomBar::compose(const ilixi::PaintEvent& event)
{
}

} /* namespace MGUI */
