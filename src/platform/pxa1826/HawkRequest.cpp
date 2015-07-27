#include <core/Logger.h>
#include <ui/BottomBar.h>
#include "HawkRequest.h"

namespace MGUI
{

D_DEBUG_DOMAIN(MGUI_HAWQREQ, "Mgui/WifiRequest", "WifiRequest");

/***************************************************************************/
/**		RilRequest Class Implementation
/***************************************************************************/
HawkRequest::HawkRequest(ubus_subscriber &subscriber, uint32_t &ubus_id, ubus_context *ubus)
 : UBusRequest(subscriber, ubus_id, ubus)
{
}

HawkRequest::~HawkRequest()
{
}

const char *HawkRequest::req_to_str(const enum PlatformRequest &req)
{
	switch (req) {
	case HawkKeepAliveReq:
		return "keep_alive";
	case HawkResetReq:
		return "reset";
	case HawkFotaReq:
		return "fota";
	case HawkNoDataAssertReq:
		return "ping";
	case HawkForceUploadReq:
		return "upload_logs";
	}
}

int HawkRequest::Request(const enum PlatformRequest &req)
{
	ILOG_INFO(MGUI_HAWQREQ, "Trigger Hawk %s\n", req_to_str(req));
	return InvokeAsync(req_to_str(req), NULL);
}

int HawkRequest::Callback(ubus_request *req, blob_attr *msg)
{
	return 0;
}

};
