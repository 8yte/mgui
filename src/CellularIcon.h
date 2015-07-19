#ifndef CELLULARICON_H_
#define CELLULARICON_H_

#include "SimpleIcon.h"

namespace MGUI
{

class CellularIcon: public SimpleIcon
{
public:
    CellularIcon(ilixi::Widget* parent = NULL);

    virtual
    ~CellularIcon();

    CellularState
    getCellularState() const;

    void
    setCellularState(CellularState cellularState);

private:
    CellularState _cellularState;
};

} /* namespace MGUI */

#endif /* CELLULARICON_H_ */
