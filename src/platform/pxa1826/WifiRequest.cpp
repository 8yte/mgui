#include <core/Logger.h>
#include <ui/StatusBar.h>
#include "WifiRequest.h"

#define WIFI_UBUS_REQUEST	"wifi_get_basic_info"

namespace MGUI
{

D_DEBUG_DOMAIN(MGUI_WIFIREQ, "Mgui/WifiRequest", "WifiRequest");

/***************************************************************************/
/**		RilRequest Class Implementation
/***************************************************************************/
WifiRequest::WifiRequest(ubus_subscriber &subscriber, uint32_t &ubus_id, ubus_context *ubus)
 : UBusRequest(subscriber, ubus_id, ubus)
{
}

WifiRequest::~WifiRequest()
{
}

int WifiRequest::Request()
{
	return InvokeAsync(WIFI_UBUS_REQUEST, NULL);
}

int WifiRequest::Callback(ubus_request *req, blob_attr *msg)
{
	return MGuiWifi::Instance()->Update(msg);
}

};
