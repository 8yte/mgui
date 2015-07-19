#include "CellularIcon.h"

namespace MGUI
{

CellularIcon::CellularIcon(ilixi::Widget* parent)
        : SimpleIcon(parent),
          _cellularState(CellularNull)
{
    setCellularState(CellularOff);
}

CellularIcon::~CellularIcon()
{
}

CellularState
CellularIcon::getCellularState() const
{
    return _cellularState;
}

void
CellularIcon::setCellularState(CellularState cellularState)
{
    if (_cellularState != cellularState)
    {
        _cellularState = cellularState;
        switch (_cellularState)
        {
            case CellularNull:
                setImage("cellular_null");
                break;
            case CellularOff:
                setImage("cellular_off");
                break;
            case Cellular0:
                setImage("cellular_0_bar");
                break;
            case Cellular0NoData:
                setImage("cellular_0_bar_no_data");
                break;
            case Cellular1:
                setImage("cellular_1_bar");
                break;
            case Cellular1NoData:
                setImage("cellular_1_bar_no_data");
                break;
            case Cellular2:
                setImage("cellular_2_bar");
                break;
            case Cellular2NoData:
                setImage("cellular_2_bar_no_data");
                break;
            case Cellular3:
                setImage("cellular_3_bar");
                break;
            case Cellular3NoData:
                setImage("cellular_3_bar_no_data");
                break;
            case Cellular4:
                setImage("cellular_4_bar");
                break;
            case Cellular4NoData:
                setImage("cellular_4_bar_no_data");
                break;
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
