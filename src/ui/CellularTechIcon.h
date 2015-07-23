#ifndef CELLULARTECHICON_H_
#define CELLULARTECHICON_H_

#include "SimpleIcon.h"

namespace MGUI
{

class CellularTechIcon: public SimpleIcon
{
public:
    CellularTechIcon(ilixi::Widget* parent = NULL);

    virtual
    ~CellularTechIcon();

    CellularTechState
    getCellularTechState() const;

    void
    setCellularTechState(CellularTechState cellularTechState);

private:
    CellularTechState _cellularTechState;
};

} /* namespace MGUI */

#endif /* CELLULARTECHICON_H_ */
