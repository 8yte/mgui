#include "SimpleIcon.h"
#include <graphics/IconPack.h>
#include <graphics/Painter.h>

namespace MGUI
{

ilixi::IconPack* SimpleIcon::__iconPack = NULL;

SimpleIcon::SimpleIcon(Widget* parent)
        : Widget(parent),
          _image(NULL)
{
    setConstraints(ilixi::FixedConstraint, ilixi::FixedConstraint);
    if (!__iconPack)
        __iconPack = new ilixi::IconPack(DATADIR"/theme/IconPack.xml");
}

SimpleIcon::~SimpleIcon()
{
    delete _image;
}

ilixi::Size
SimpleIcon::preferredSize() const
{
    return ilixi::Size(24, 24);
}

void
SimpleIcon::setImage(const std::string& name)
{
    if (_image)
        delete _image;
    _image = __iconPack->getIcon(name);
    update();
}

void
SimpleIcon::compose(const ilixi::PaintEvent& event)
{
    if (_image)
    {
        ilixi::Painter p(this);
        p.begin(event);
        p.drawImage(_image, 0, 0);
        p.end();
    }
}

} /* namespace MGUI */
