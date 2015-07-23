#include "CellularTechIcon.h"

namespace MGUI
{

CellularTechIcon::CellularTechIcon(ilixi::Widget* parent)
        : SimpleIcon(parent),
          _cellularTechState(CellularTechUnknown)
{
    setCellularTechState(CellularTechUnknown);
}

CellularTechIcon::~CellularTechIcon()
{
}

CellularTechState
CellularTechIcon::getCellularTechState() const
{
    return _cellularTechState;
}

void
CellularTechIcon::setCellularTechState(CellularTechState cellularTechState)
{
    if (_cellularTechState != cellularTechState)
    {
        _cellularTechState = cellularTechState;
        switch (_cellularTechState)
        {
            case CellularTechUnknown:
                setImage("cellular_tech_unknown");
                break;
            case CellularTechHSDPAP:
                setImage("cellular_tech_hsdpa+");
                break;
            case CellularTechHSDPA:
                setImage("cellular_tech_hsdpa");
                break;
            case CellularTechEdge:
                setImage("cellular_tech_edge");
                break;
            case CellularTechG:
                setImage("cellular_tech_g");
                break;
            case CellularTech2G:
                setImage("cellular_tech_2g");
                break;
            case CellularTech3G:
                setImage("cellular_tech_3g");
                break;
            case CellularTech4G:
                setImage("cellular_tech_4g");
                break;
            default:
                break;
        }
    }
}

} /* namespace MGUI */
