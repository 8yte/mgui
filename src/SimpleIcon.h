#ifndef SIMPLEICON_H_
#define SIMPLEICON_H_

#include <ui/Widget.h>

#include "MGuiPlatform.h"

namespace ilixi
{
class IconPack;
}

namespace MGUI
{

class SimpleIcon : public ilixi::Widget
{
public:
    SimpleIcon(ilixi::Widget* parent = NULL);

    virtual
    ~SimpleIcon();

    ilixi::Size
    preferredSize() const;

    void
    setImage(const std::string& name);

protected:
    virtual void
    compose(const ilixi::PaintEvent& event);

private:
    static ilixi::IconPack* __iconPack;
    ilixi::Image* _image;
};

} /* namespace MGUI */

#endif /* SIMPLEICON_H_ */
