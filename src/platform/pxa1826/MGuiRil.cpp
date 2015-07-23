#include <core/Logger.h>
#include <ui/StatusBar.h>
#include "MGuiRil.h"

#define RIL_UBUS_ID		"ril"

namespace MGUI
{

D_DEBUG_DOMAIN(MGUI_RIL, "Mgui/Ril", "Ril");

/***************************************************************************/
/**		MGuiRil Class Implementation
/***************************************************************************/
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

MGuiRil *MGuiRil:: _instance = 0;

static int ril_indication(struct ubus_context *ctx, struct ubus_object *obj,
			      struct ubus_request_data *req,
			      const char *method, struct blob_attr *msg)
{
	MGuiRil *__this = MGuiRil::Instance();
	if (!__this) {
		ILOG_ERROR(MGUI_RIL, "No instance, indication ignored!\n");
		return -1;
	}
		
	return __this->Indication(ctx, obj, req, method, msg);
}

enum {
	REQID,
	ERRNO,
	DATA,
	_MAX
};

#define blob_for_each_attr(pos, attr, rem) \
	for (rem = attr ? blob_len(attr) : 0, \
	     pos = attr ? (blob_attr*)blob_data(attr) : 0; \
	     rem > 0 && (blob_pad_len(pos) <= rem) && \
	     (blob_pad_len(pos) >= sizeof(struct blob_attr)); \
	     rem -= blob_pad_len(pos), pos = blob_next(pos))

int MGuiRil:: parse_blobmsg(struct blob_attr *attr, struct blob_attr **tb)
{
	blob_attr *pos = NULL;
	blobmsg_hdr *hdr = NULL;
	const char *policy[] = { "rilid", "resperrno" };
	int rem = 0;
	int i = 0;

	blob_for_each_attr(pos, attr, rem) {
		if (i > _MAX)
			return 0;

		hdr = (blobmsg_hdr *)blob_data(pos);
		if (i == 0) {
			if (strcmp(policy[i], (char *)hdr->name) != 0) {
				ILOG_ERROR(MGUI_RIL, "format error\n");
				return -1;
			}
		}

		tb[i] = pos;
		i++;
	}

	return 0;
}

int MGuiRil::Indication(struct ubus_context *ctx, struct ubus_object *obj,
			      struct ubus_request_data *req,
			      const char *method, struct blob_attr *msg)
{
	unsigned int id = 0;
	unsigned int errcode = 0;
	void *data = NULL;
	int len = 0;
	int ret = 0;
	blob_attr *tb[_MAX] = { 0 };

	ILOG_TRACE_F(MGUI_RIL);

	ret = parse_blobmsg(msg, tb);
	if (ret < 0) {
		ILOG_ERROR(MGUI_RIL, "parse msg error\n");
		return -1;
	}

	if (tb[REQID])
		id = blobmsg_get_u32(tb[REQID]);
	if (tb[ERRNO]) {
		errcode = blobmsg_get_u32(tb[ERRNO]);
		if (errcode) {
			ILOG_ERROR(MGUI_RIL, "unsolicited id %d blobmsg err %s\n",
				  id, errcode);
			return -1;
		}
	}
	if (tb[DATA]) {
		data = blob_data(tb[DATA]);
		len = blobmsg_data_len(tb[DATA]);
	}

	ILOG_DEBUG(MGUI_RIL, "id is %d, data = %p, len = %d\n", id, data, len);

	switch(id)
	{
		case RIL_UNSOL_SIGNAL_STRENGTH:
			ILOG_DEBUG(MGUI_RIL,"Signal Strength indication\n");
			_registration->Indication(id, data, len);
			break;
		case RIL_UNSOL_RESPONSE_SIM_STATUS_CHANGED:
			ILOG_DEBUG(MGUI_RIL,"Sim state changed\n");
		case RIL_UNSOL_RESPONSE_RADIO_STATE_CHANGED:
			ILOG_DEBUG(MGUI_RIL,"Radio state changed\n");
		case RIL_UNSOL_RESPONSE_VOICE_NETWORK_STATE_CHANGED:
			ILOG_DEBUG(MGUI_RIL,"Voice registration status changed\n");
		case RIL_UNSOL_VOICE_RADIO_TECH_CHANGED:
			ILOG_DEBUG(MGUI_RIL,"Radio technology changed\n");
			SendRequests();
			break;
		default:
			ILOG_DEBUG(MGUI_RIL, "id of %d is not supported\n",id);
			ret = 0;
	}

	ILOG_DEBUG(MGUI_RIL, "%s exit\n", __FUNCTION__);
	return 0;
}

MGuiRil* MGuiRil::Instance()
{
	return _instance;
}

int MGuiRil::Create(ubus_context *ubus, StatusBar *bar)
{
	int ret;

	ILOG_TRACE_F(MGUI_RIL);
	if (_instance) {
		ILOG_ERROR(MGUI_RIL, "Already created\n");
		return -1;
	}

	_instance = new MGuiRil(ubus, bar);
	ret = _instance->Register();
	if (ret) {
		ILOG_ERROR(MGUI_RIL, "Register failed (ret=%d)\n", ret);
		goto out_error;
	}

	ILOG_DEBUG(MGUI_RIL, "%s exit\n", __FUNCTION__);
	return 0;

out_error:
	delete _instance;
	_instance = NULL;
}


void MGuiRil::Destroy()
{
	ILOG_TRACE_F(MGUI_RIL);

	if (_instance) {
		_instance->UnRegister();
		delete _instance->_operator;
		delete _instance->_simcard;
		delete _instance->_registration;
		delete _instance->_screen;
		delete _instance;
		_instance = NULL;
	}
}

MGuiRil::MGuiRil(ubus_context *ubus, StatusBar *bar)
 : UBusClient(ubus)
{
	ILOG_TRACE(MGUI_RIL);
	_bar = bar;
	ILOG_DEBUG(MGUI_RIL, "%s exit\n", __FUNCTION__);
}

MGuiRil::~MGuiRil()
{
	ILOG_TRACE(MGUI_RIL);
	ILOG_DEBUG(MGUI_RIL, "%s exit\n", __FUNCTION__);
}

void MGuiRil::SendRequests()
{
	ILOG_TRACE(MGUI_RIL);
	if (_instance) {
		_simcard->Request();
		_operator->Request();
		_registration->Request();
		_screen->Request(1);
	}
	ILOG_DEBUG(MGUI_RIL, "%s exit\n", __FUNCTION__);
}

int MGuiRil::Register()
{
	int ret;

	ILOG_TRACE(MGUI_RIL);

	ret = UBusClient::Register(ril_indication, RIL_UBUS_ID);
	if (ret) {
		ILOG_ERROR(MGUI_RIL, "Register failed %d\n", ret);
		return ret;
	}

	ret = UBusClient::Subscribe(_events);
	if (ret) {
		ILOG_ERROR(MGUI_RIL, "Subscribe events failed %d\n", ret);
		goto unregister;
	}

	_operator = new RilOperator(_subscriber, _id, _ubus, _bar);
	_simcard = new RilSimcard(_subscriber, _id, _ubus, _bar);
	_registration = new RilRegistration(_subscriber, _id, _ubus, _bar);
	_screen = new RilScreen(_subscriber, _id, _ubus);

	SendRequests();

	ILOG_DEBUG(MGUI_RIL, "%s exit\n", __FUNCTION__);
	return 0;

unregister:
	UBusClient::UnRegister();
	return ret;
}

int MGuiRil::UnRegister()
{
	ILOG_TRACE(MGUI_RIL);
	_screen->Request(0);
	return UBusClient::UnRegister();
}

};
