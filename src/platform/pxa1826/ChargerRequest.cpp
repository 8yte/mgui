#include <core/Logger.h>
#include <ui/StatusBar.h>
#include "ChargerRequest.h"

#define CHARGER_UBUS_REQUEST	"get_chg_info"

namespace MGUI
{

D_DEBUG_DOMAIN(MGUI_CHARGERREQ, "Mgui/ChargerRequest", "ChargerRequest");

/***************************************************************************/
/**		RilRequest Class Implementation
/***************************************************************************/
ChargerRequest::ChargerRequest(ubus_subscriber &subscriber, uint32_t &ubus_id, ubus_context *ubus)
 : UBusRequest(subscriber, ubus_id, ubus)
{
}

ChargerRequest::~ChargerRequest()
{
}

int ChargerRequest::Request()
{
	return InvokeAsync(CHARGER_UBUS_REQUEST, NULL);
}

int ChargerRequest::Callback(ubus_request *req, blob_attr *msg)
{
	return MGuiCharger::Instance()->Update(msg);
}

};
