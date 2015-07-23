#ifndef BOTTOMBAR_H_
#define BOTTOMBAR_H_

#include <ui/ContainerBase.h>
#include <ui/Label.h>

namespace MGUI
{

class BottomBar : public ilixi::ContainerBase
{
public:
    BottomBar(ilixi::Widget* parent = NULL);

    virtual
    ~BottomBar();

protected:
    virtual void
    compose(const ilixi::PaintEvent& event);

private:
    ilixi::Label* _version;
};

} /* namespace MGUI */

#endif /* BOTTOMBAR_H_ */
