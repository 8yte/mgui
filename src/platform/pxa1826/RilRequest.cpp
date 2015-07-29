#include <core/Logger.h>
#include <ui/StatusBar.h>
#include <ui/ToolButton.h>
#include "MGuiRil.h"
#include "RilRequest.h"

#define RIL_UBUS_REQ		"ril_request"

namespace MGUI
{

D_DEBUG_DOMAIN(MGUI_RILREQ, "Mgui/RilRequest", "RilRequest");
D_DEBUG_DOMAIN(MGUI_OPERATOR, "Mgui/RilRequest/Operator", "OperatorRequest");
D_DEBUG_DOMAIN(MGUI_SIMCARD, "Mgui/RilRequest/Simcard", "SimcardRequest");
D_DEBUG_DOMAIN(MGUI_SCREEN, "Mgui/RilRequest/Screen", "ScreenRequest");
D_DEBUG_DOMAIN(MGUI_REGISTRATION, "Mgui/RilRequest/Registration", "RegistrationRequest");
D_DEBUG_DOMAIN(MGUI_INDICATION, "Mgui/RilIndication", "RilIndication");

/***************************************************************************/
/**		RilRequest Class Implementation
/***************************************************************************/
RilRequest::RilRequest(ubus_subscriber &subscriber, uint32_t &ubus_id, ubus_context *ubus)
 : UBusRequest(subscriber, ubus_id, ubus)
{
}

RilRequest::~RilRequest()
{
}

int RilRequest::Request(uint32_t id, const void *data, int len)
{
	int ret = 0;

	ILOG_TRACE(MGUI_RILREQ);
	ret = rilutil_makeRequestBlob(&_b, id, data, len);
	if (ret) {
		ILOG_ERROR(MGUI_RILREQ, "rilutil_makeRequestBlob failed (ret=%d)\n", ret);
		return ret;
	}
	ret = InvokeAsync(RIL_UBUS_REQ, _b.head);
	if (ret) {
		ILOG_ERROR(MGUI_RILREQ, "Invoke() failed (ret=%d)\n", ret);
		return ret;
	}
	ILOG_DEBUG(MGUI_RILREQ, "%s exit\n", __FUNCTION__);

	return ret;
}

int RilRequest::Request(uint32_t id)
{
	return Request(id, 0, 0);
}

/***************************************************************************/
/**		RilOperator Class Implementation
/***************************************************************************/
RilOperator::RilOperator(ubus_subscriber &subscriber, uint32_t &ubus_id, ubus_context* ubus)
	: RilRequest(subscriber, ubus_id, ubus)
{
}

RilOperator::~RilOperator()
{
}

int RilOperator::Request()
{
	ILOG_DEBUG(MGUI_OPERATOR, "Request operator\n");
	return RilRequest::Request(RIL_REQUEST_OPERATOR);
}

int RilOperator::Callback(ubus_request *req, blob_attr *msg)
{
	ILOG_TRACE(MGUI_OPERATOR);
	if(!req || !msg) {
		ILOG_ERROR(MGUI_OPERATOR, "request or message is NULL! (req=%p, msg=%p)", req, msg);
		return -1;
	}
	return MGuiRil::Instance()->OperatorCallback(msg);
}

/***************************************************************************/
/**		RilSimcard Class Implementation
/***************************************************************************/
RilSimcard::RilSimcard(ubus_subscriber &subscriber, uint32_t &ubus_id, ubus_context* ubus)
	: RilRequest(subscriber, ubus_id, ubus)
{
}

RilSimcard::~RilSimcard()
{
}

int RilSimcard::Request()
{
	ILOG_DEBUG(MGUI_SIMCARD, "Request simcard status\n");
	return RilRequest::Request(RIL_REQUEST_GET_SIM_STATUS);
}

int RilSimcard::Callback(ubus_request *req, blob_attr *msg)
{
	ILOG_TRACE(MGUI_SIMCARD);
	if(!req || !msg) {
		ILOG_ERROR(MGUI_SIMCARD, "request or message is NULL! (req=%p, msg=%p)", req, msg);
		return -1;
	}
	return MGuiRil::Instance()->SimCallback(msg);
}

/***************************************************************************/
/**		RilScreen Class Implementation
/***************************************************************************/

RilScreen::RilScreen(ubus_subscriber &subscriber, uint32_t &ubus_id, ubus_context* ubus)
	: RilRequest(subscriber, ubus_id, ubus)
{
}

RilScreen::~RilScreen()
{
}

int RilScreen::Request(int on_off)
{
	ILOG_DEBUG(MGUI_SCREEN, "Request screen %s\n",on_off ? "on" : "off");
	return RilRequest::Request(RIL_REQUEST_SCREEN_STATE, (const void *)&on_off, (int)1);
}

int RilScreen::Callback(ubus_request *req, blob_attr *msg)
{
	return 0;
}

/***************************************************************************/
/**		RilRegistration Class Implementation
/***************************************************************************/
RilRegistration::RilRegistration(ubus_subscriber &subscriber, uint32_t &ubus_id, ubus_context* ubus)
	: RilRequest(subscriber, ubus_id, ubus)
{
}

RilRegistration::~RilRegistration()
{
}

int RilRegistration::Request()
{
	int ret;

	ILOG_TRACE(MGUI_REGISTRATION);

	ILOG_DEBUG(MGUI_REGISTRATION, "Request voice registration\n");
	ret = RilRequest::Request(RIL_REQUEST_VOICE_REGISTRATION_STATE);
	if (ret) {
		ILOG_ERROR(MGUI_RILREQ, "request voice registration failed (ret=%d)\n", ret);
		return ret;
	}

	ILOG_DEBUG(MGUI_REGISTRATION, "Request data registration\n");
	ret = RilRequest::Request(RIL_REQUEST_DATA_REGISTRATION_STATE);
	if (ret) {
		ILOG_ERROR(MGUI_REGISTRATION, "request voice registration failed (ret=%d)\n", ret);
		return ret;
	}

	ILOG_DEBUG(MGUI_REGISTRATION, "%s:%d: Exit\n", __FUNCTION__, __LINE__);

	return 0;
}

int RilRegistration::Callback(ubus_request *req, blob_attr *msg)
{
	ILOG_TRACE(MGUI_REGISTRATION);
	if(!req || !msg) {
		ILOG_ERROR(MGUI_REGISTRATION, "request or msg is NULL (req=%p, msg=%p)\n", req, msg);
		return -1;
	}
	return MGuiRil::Instance()->RegistrationCallback(msg);
}

};
