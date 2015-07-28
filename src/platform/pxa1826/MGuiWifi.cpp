#include <core/Logger.h>
#include <ui/StatusBar.h>
#include <ui/ToolButton.h>
#include "MGuiWifi.h"

#define WIFI_UBUS_ID			"wireless"

//#define STATISTICS_UBUS_ID		"statistics"
//#define STATISTICS_UBUS_REQUEST	"get_wifi_status"

namespace MGUI
{

D_DEBUG_DOMAIN(MGUI_WIFI, "Mgui/Wifi", "Wifi");

MGuiWifi *MGuiWifi:: _instance = 0;

const blobmsg_policy MGuiWifi::_policy[] = {
	{ "wireless", BLOBMSG_TYPE_TABLE },
	{ "wifi_if_24G", BLOBMSG_TYPE_TABLE },
	{ "wifi_if_5G", BLOBMSG_TYPE_TABLE },
	{ "device", BLOBMSG_TYPE_STRING },
	{ "switch", BLOBMSG_TYPE_STRING },
	{ "channel", BLOBMSG_TYPE_STRING },
	{ "ssid", BLOBMSG_TYPE_STRING },
};

static int wifi_indication(struct ubus_context *ctx, struct ubus_object *obj,
			      struct ubus_request_data *req,
			      const char *method, struct blob_attr *msg)
{
	MGuiWifi *__this = MGuiWifi::Instance();
	if (!__this) {
		ILOG_ERROR(MGUI_WIFI, "No instance, indication ignored!\n");
		return -1;
	}
		
	return __this->Indication(ctx, obj, req, method, msg);
}

#undef blob_for_each_attr
#define blob_for_each_attr(pos, attr, rem) \
	for (rem = attr ? blob_len(attr) : 0, \
	     pos = attr ? (blob_attr*)blob_data(attr) : 0; \
	     rem > 0 && (blob_pad_len(pos) <= rem) && \
	     (blob_pad_len(pos) >= sizeof(struct blob_attr)); \
	     rem -= blob_pad_len(pos), pos = blob_next(pos))

int MGuiWifi::parseattrs_tag(blob_attr *msg)
{

	struct blob_attr *tb[ARRAY_SIZE(_policy)];
	int ret;

	ret =  blobmsg_parse(_policy, ARRAY_SIZE(_policy),
			     tb, blob_data(msg), blob_len(msg));
	if (ret){
		ILOG_ERROR(MGUI_WIFI, "parsing blobmsg failed %d\n", ret);
		return -1;
	}

	if (!tb[ATTR_WIRELESS])
		return -1;

	ret =  blobmsg_parse(_policy, ARRAY_SIZE(_policy), tb,
			     blobmsg_data(tb[ATTR_WIRELESS]),
			     blobmsg_data_len(tb[ATTR_WIRELESS]));
	if (ret){
		ILOG_ERROR(MGUI_WIFI, "parsing blobmsg failed %d\n", ret);
		return -1;
	}

	if (!tb[ATTR_WIFI_IF_2G] && !tb[ATTR_WIFI_IF_5G]) {
		ILOG_ERROR(MGUI_WIFI, "parsing blobmsg failed %d\n", ret);
		return -1;
	}

	if (tb[ATTR_WIFI_IF_2G])
		ret =  blobmsg_parse(_policy,
				     ARRAY_SIZE(_policy), tb,
				     blobmsg_data(tb[ATTR_WIFI_IF_2G]),
				     blobmsg_data_len(tb[ATTR_WIFI_IF_2G]));
	else if (tb[ATTR_WIFI_IF_5G])
		ret =  blobmsg_parse(_policy,
				     ARRAY_SIZE(_policy), tb,
				     blobmsg_data(tb[ATTR_WIFI_IF_5G]),
				     blobmsg_data_len(tb[ATTR_WIFI_IF_5G]));
	if (ret){
		ILOG_ERROR(MGUI_WIFI, "parsing blobmsg failed %d\n", ret);
		return -1;
	}

	if (tb[ATTR_DEVICE])
		_status.device.assign(blobmsg_get_string(tb[ATTR_DEVICE]));
	if (tb[ATTR_SWITCH])
		_status.on.assign(blobmsg_get_string(tb[ATTR_SWITCH]));
	if (tb[ATTR_CHANNEL])
		_status.channel.assign(blobmsg_get_string(tb[ATTR_CHANNEL]));
	if (tb[ATTR_SSID])
		_status.ssid.assign(blobmsg_get_string(tb[ATTR_SSID]));

	return 0;
}

int MGuiWifi::Update(blob_attr *msg)
{
	int ret = parseattrs_tag(msg);
	if (ret){
		ILOG_ERROR(MGUI_WIFI, "parsing blobmsg failed %d\n", ret);
		return ret;
	}

	_button->setText(_status.ssid);
	ILOG_INFO(MGUI_WIFI, "WIFI info: device=%s, on=%s, channel=%s, ssid=%s\n",
		  _status.device.c_str(), _status.on.c_str(),
		  _status.channel.c_str(), _status.ssid.c_str());
	return 0;
}

int MGuiWifi::Indication(struct ubus_context *ctx, struct ubus_object *obj,
			      struct ubus_request_data *req,
			      const char *method, struct blob_attr *msg)
{
	return Update(msg);
}

MGuiWifi* MGuiWifi::Instance()
{
	return _instance;
}

int MGuiWifi::Create(ubus_context *ubus, StatusBar *bar, ilixi::ToolButton* button)
{
	int ret;

	ILOG_TRACE_F(MGUI_WIFI);
	if (_instance) {
		ILOG_ERROR(MGUI_WIFI, "Already created\n");
		return -1;
	}

	_instance = new MGuiWifi(ubus, bar, button);
	ret = _instance->Register();
	if (ret) {
		ILOG_ERROR(MGUI_WIFI, "Register failed (ret=%d)\n", ret);
		goto out_error;
	}
	_instance->Register();

	ILOG_DEBUG(MGUI_WIFI, "%s exit\n", __FUNCTION__);
	return 0;

out_error:
	delete _instance;
	_instance = NULL;
}


void MGuiWifi::Destroy()
{
	ILOG_TRACE_F(MGUI_WIFI);

	if (_instance) {
		_instance->UnRegister();
		_instance = NULL;
	}
}

MGuiWifi::MGuiWifi(ubus_context *ubus, StatusBar *bar, ilixi::ToolButton* button)
 : UBusClient(ubus),
   _bar(bar),
   _button(button)
{
}

MGuiWifi::~MGuiWifi()
{
}

int MGuiWifi::Register()
{
	int ret;

	ILOG_TRACE(MGUI_WIFI);

	ret = UBusClient::Register(wifi_indication, WIFI_UBUS_ID);
	if (ret) {
		ILOG_ERROR(MGUI_WIFI, "Register failed %d\n", ret);
		return ret;
	}

	ret = UBusClient::Subscribe();
	if (ret) {
		ILOG_ERROR(MGUI_WIFI, "Subscribe events failed %d\n", ret);
		goto unregister;
	}

	_request = new WifiRequest(_subscriber, _id, _ubus);
	_request->Request();

	ILOG_DEBUG(MGUI_WIFI, "%s exit\n", __FUNCTION__);
	return 0;

unregister:
	UBusClient::UnRegister();
	return ret;
}

int MGuiWifi::UnRegister()
{
	return UBusClient::UnRegister();
}

};
