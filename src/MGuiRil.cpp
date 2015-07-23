#include <core/Logger.h>
#include "MGuiRil.h"

namespace MGUI
{

D_DEBUG_DOMAIN(MGUI_RIL, "Mgui/Ril", "Ril");

static int RilIndication(struct ubus_context *ctx, struct ubus_object *obj,
			      struct ubus_request_data *req,
			      const char *method, struct blob_attr *msg)
{
	
//	ILOG_TRACE(MGUI_RIL);
//	ILOG_ERROR(MGUI_RIL, "%s method=%s\n", __func__, method);
//	ILOG_DEBUG(MGUI_RIL, "%s exit\n", __func__);
	return 0;
}

const std::vector<std::string> MGuiRil:: _events({
	"ril.unsol.cc",
	"ril.unsol.dev",
	"ril.unsol.mm",
	"ril.unsol.msg",
	"ril.unsol.ps",
	"ril.unsol.sim",
	"ril.unsol.ss",
	"ril.unsol"
});

MGuiRil::MGuiRil(ubus_context* ubus)
 : UBusSubscriber(ubus)
{
	ILOG_TRACE(MGUI_RIL);
	ILOG_DEBUG(MGUI_RIL, "%s exit\n", __func__);
}

MGuiRil::~MGuiRil()
{
	ILOG_TRACE(MGUI_RIL);
	ILOG_DEBUG(MGUI_RIL, "%s exit\n", __func__);
}

int MGuiRil::Register(ubus_handler_t cb, const char *name)
{
	ILOG_TRACE(MGUI_RIL);
	return UBusSubscriber::Register(RilIndication, "ril");
}

int MGuiRil::UnRegister()
{
	ILOG_TRACE(MGUI_RIL);
	return UBusSubscriber::UnRegister();
}

int MGuiRil::Subscribe(std::string &event)
{
	ILOG_TRACE(MGUI_RIL);
	return UBusSubscriber::Subscribe(event);
}

int MGuiRil::UnSubscribe(std::string &event)
{
	ILOG_TRACE(MGUI_RIL);
	return UBusSubscriber::UnSubscribe(event);
}

int MGuiRil::Subscribe(std::vector<std::string> &events)
{
	ILOG_TRACE(MGUI_RIL);
	//return UBusSubscriber::Subscribe(&_events);
	return 0;
}

int MGuiRil::UnSubscribe(std::vector<std::string> &events)
{
	ILOG_TRACE(MGUI_RIL);
	//return UBusSubscriber::UnSubscribe(_events);
	return 0;
}

};
