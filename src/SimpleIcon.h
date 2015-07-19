/*
 * SimpleIcon.h
 *
 *  Created on: Jul 17, 2015
 *      Author: tarik
 */

#ifndef SIMPLEICON_H_
#define SIMPLEICON_H_

#include <ui/Widget.h>

namespace ilixi
{
class IconPack;
}

namespace MGUI
{

class SimpleIcon : public ilixi::Widget
{
public:
    SimpleIcon(Widget* parent = NULL);

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
