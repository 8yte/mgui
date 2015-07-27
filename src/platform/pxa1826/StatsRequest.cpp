#include <core/Logger.h>
#include <ui/StatusBar.h>
#include "StatsRequest.h"

#define STATISTICS_UBUS_REQUEST	"get_wifi_status"

namespace MGUI
{

D_DEBUG_DOMAIN(MGUI_STATSREQ, "Mgui/StatsRequest", "StatsRequest");

/***************************************************************************/
/**		RilRequest Class Implementation
/***************************************************************************/
StatsRequest::StatsRequest(ubus_subscriber &subscriber, uint32_t &ubus_id, ubus_context *ubus)
 : UBusRequest(subscriber, ubus_id, ubus)
{
}

StatsRequest::~StatsRequest()
{
}

int StatsRequest::Request()
{
	return InvokeAsync(STATISTICS_UBUS_REQUEST, NULL);
}

int StatsRequest::Callback(ubus_request *req, blob_attr *msg)
{
	return MGuiStats::Instance()->Update(msg);
}

};
