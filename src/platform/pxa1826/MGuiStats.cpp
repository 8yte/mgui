#include <core/Logger.h>
#include <ui/StatusBar.h>
#include "MGuiStats.h"

#define STATISTICS_UBUS_ID		"statistics"

namespace MGUI
{

D_DEBUG_DOMAIN(MGUI_STATS, "Mgui/Stats", "Statistics");

MGuiStats *MGuiStats:: _instance = 0;

const blobmsg_policy MGuiStats::_policy[] = {
	{ "statistics", BLOBMSG_TYPE_TABLE },
	{ "wifi_status", BLOBMSG_TYPE_STRING },
};

static int stats_indication(struct ubus_context *ctx, struct ubus_object *obj,
			      struct ubus_request_data *req,
			      const char *method, struct blob_attr *msg)
{
	MGuiStats *__this = MGuiStats::Instance();
	if (!__this) {
		ILOG_ERROR(MGUI_STATS, "No instance, indication ignored!\n");
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

int MGuiStats::parseattrs_req(blob_attr *msg)
{

	struct blob_attr *tb[ARRAY_SIZE(_policy)];
	int ret;

	ret =  blobmsg_parse(_policy, ARRAY_SIZE(_policy),
			     tb, blob_data(msg), blob_len(msg));
	if (ret){
		ILOG_ERROR(MGUI_STATS, "parsing blobmsg failed %d\n", ret);
		return -1;
	}

	if (!tb[ATTR_STATISTICS])
		return -1;

	ret =  blobmsg_parse(_policy, ARRAY_SIZE(_policy), tb,
			     blobmsg_data(tb[ATTR_STATISTICS]),
			     blobmsg_data_len(tb[ATTR_STATISTICS]));
	if (ret){
		ILOG_ERROR(MGUI_STATS, "parsing blobmsg failed %d\n", ret);
		return -1;
	}

	if (!tb[ATTR_WIFI_STATUS]) {
		ILOG_ERROR(MGUI_STATS, "parsing blobmsg failed %d\n", ret);
		return -1;
	}

	if (tb[ATTR_WIFI_STATUS])
		_status.wifi_status.assign(blobmsg_get_string(tb[ATTR_WIFI_STATUS]));
	else
		_status.wifi_status.assign("unknown");

	return 0;
}

int MGuiStats::Update(blob_attr *msg)
{
	int ret = parseattrs_req(msg);
	if (ret){
		ILOG_ERROR(MGUI_STATS, "parsing blobmsg failed %d\n", ret);
		return ret;
	}

	if (!strcmp(_status.wifi_status.c_str(), "ready"))
		_bar->setWifiState(Wifi4Bar); //TODO - get wifi signal strength
	else
		_bar->setWifiState(WifiOff);
	
	ILOG_INFO(MGUI_STATS, "wifi status %s\n", _status.wifi_status.c_str());

	return 0;
}

int MGuiStats::parseattrs_ind(struct blob_attr *attr, struct blob_attr **tb)
{
	blob_attr *pos = NULL;
	blobmsg_hdr *hdr = NULL;
	const char *policy[] = {"id"};
	int rem = 0;
	int i = 0;

	blob_for_each_attr(pos, attr, rem) {
		if (i > _MAX)
			return 0;

		hdr = (blobmsg_hdr*)blob_data(pos);
		if (i == 0) {
			if (strcmp(policy[i], (const char *)hdr->name) != 0) {
				ILOG_ERROR(MGUI_STATS, "format error\n");
				return -1;
			}
		}

		tb[i] = pos;
		i++;
	}

	return 0;
}

int MGuiStats::Indication(struct ubus_context *ctx, struct ubus_object *obj,
			      struct ubus_request_data *req,
			      const char *method, struct blob_attr *msg)
{
	struct blob_attr *tb[_MAX] = { 0 };
	unsigned int id;
	struct blob_attr *attr;
	int ret = 0;

	ret = parseattrs_ind(msg, tb);
	if (ret < 0) {
		ILOG_ERROR(MGUI_STATS, "parse msg error\n");
		return -1;
	}

	if (!tb[REQID] || !tb[DATA]) {
		ILOG_ERROR(MGUI_STATS, "Error: id or data not found\n");
		return -1;
	}

	id = blobmsg_get_u32(tb[REQID]);
	attr = tb[DATA];

	ILOG_DEBUG(MGUI_STATS, "id is %d, attr = %p\n", id, attr);

	switch (id) {
	case WIFI_CLIENTS_NOTIFY_ID:
		_status.num_clients = atoi(blobmsg_get_string(attr));
		//TODO update GUI display
		break;
	case WIFI_STATUS_NOTIFY_ID:
		_status.wifi_status.assign(blobmsg_get_string(attr));
		break;
	default:
		ILOG_ERROR(MGUI_STATS, "id of %d is not supported\n",id);
	}

	if (!strcmp(_status.wifi_status.c_str(), "ready"))
		_bar->setWifiState(Wifi4Bar); //TODO - set according to wifi signal strength
	else
		_bar->setWifiState(WifiOff);

	return 0;
}

MGuiStats* MGuiStats::Instance()
{
	return _instance;
}

int MGuiStats::Create(ubus_context *ubus, StatusBar *bar)
{
	int ret;

	ILOG_TRACE_F(MGUI_STATS);
	if (_instance) {
		ILOG_ERROR(MGUI_STATS, "Already created\n");
		return -1;
	}

	_instance = new MGuiStats(ubus, bar);
	ret = _instance->Register();
	if (ret) {
		ILOG_ERROR(MGUI_STATS, "Register failed (ret=%d)\n", ret);
		goto out_error;
	}
	_instance->Register();

	ILOG_DEBUG(MGUI_STATS, "%s exit\n", __FUNCTION__);
	return 0;

out_error:
	delete _instance;
	_instance = NULL;
}


void MGuiStats::Destroy()
{
	ILOG_TRACE_F(MGUI_STATS);

	if (_instance) {
		_instance->UnRegister();
		_instance = NULL;
	}
}

MGuiStats::MGuiStats(ubus_context *ubus, StatusBar *bar)
 : UBusClient(ubus),
   _bar(bar)
{
}

MGuiStats::~MGuiStats()
{
}

int MGuiStats::Register()
{
	int ret;

	ILOG_TRACE(MGUI_STATS);

	ret = UBusClient::Register(stats_indication, STATISTICS_UBUS_ID);
	if (ret) {
		ILOG_ERROR(MGUI_STATS, "Register failed %d\n", ret);
		return ret;
	}

	ret = UBusClient::Subscribe();
	if (ret) {
		ILOG_ERROR(MGUI_STATS, "Subscribe events failed %d\n", ret);
		goto unregister;
	}

	_request = new StatsRequest(_subscriber, _id, _ubus);
	_request->Request();

	ILOG_DEBUG(MGUI_STATS, "%s exit\n", __FUNCTION__);
	return 0;

unregister:
	UBusClient::UnRegister();
	return ret;
}

int MGuiStats::UnRegister()
{
	return UBusClient::UnRegister();
}

};
