#include <core/Logger.h>
#include <ui/StatusBar.h>
#include "MGuiCharger.h"

#define CHARGER_UBUS_ID	"charger"
#define CHARGER_UBUS_REQUEST	"get_chg_info"

namespace MGUI
{

D_DEBUG_DOMAIN(MGUI_CHARGER, "Mgui/Charger", "Charger");

MGuiCharger *MGuiCharger:: _instance = 0;

static int charger_indication(struct ubus_context *ctx, struct ubus_object *obj,
			      struct ubus_request_data *req,
			      const char *method, struct blob_attr *msg)
{
	MGuiCharger *__this = MGuiCharger::Instance();
	if (!__this) {
		ILOG_ERROR(MGUI_CHARGER, "No instance, indication ignored!\n");
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

int MGuiCharger:: parse_blobmsg(struct blob_attr *attr, struct blob_attr **tb)
{
	blob_attr *pos = NULL;
	blobmsg_hdr *hdr = NULL;
	const char *policy[] = { "capacity", "type", "present", "health",
				 "voltage_now", "bat_temp", "bat_status",
				 "bat_technology", "acchg_online" , "acchg_type",
				  "acchg_status", "usbchg_online", "usbchg_type",
				  "usbchg_status"};
	int rem = 0;
	int i = 0;

	blob_for_each_attr(pos, attr, rem) {
		if (i > NR_ATTR)
			return 0;

		hdr = (blobmsg_hdr *)blob_data(pos);
		if (i == 0) {
			if (strcmp(policy[i], (char *)hdr->name) != 0) {
				ILOG_ERROR(MGUI_CHARGER, "format error\n");
				return -1;
			}
		}

		tb[i] = pos;
		i++;
	}

	return 0;
}

int MGuiCharger::parseattrs_tag(blob_attr *msg)
{
	int ret;

	blob_attr *tb[NR_ATTR] = { 0 };

	ret = parse_blobmsg(msg, tb);
	if (ret < 0) {
		ILOG_ERROR(MGUI_CHARGER, "parse msg error\n");
		return ret;
	}
	
	_status.capacity = tb[ATTR_CAPACITY] ?
		blobmsg_get_u32(tb[ATTR_CAPACITY]) : 0;
	_status.type = tb[ATTR_TYPE] ?
		blobmsg_get_u32(tb[ATTR_TYPE]) : 0;
	_status.present = tb[ATTR_PRESENT] ?
		blobmsg_get_u32(tb[ATTR_PRESENT]) : 0;
	_status.health = tb[ATTR_HEALTH] ?
		blobmsg_get_u32(tb[ATTR_HEALTH]) : 0;
	_status.voltage_now = tb[ATTR_VOLTAGE] ?
		blobmsg_get_u32(tb[ATTR_VOLTAGE]) : 0;
	_status.bat_temp = tb[ATTR_TEMP] ?
		blobmsg_get_u32(tb[ATTR_TEMP]) : 0;
	_status.bat_status = tb[ATTR_STATUS] ?
		blobmsg_get_u32(tb[ATTR_STATUS]) : 0;
	_status.bat_technology = tb[ATTR_TECH] ?
		blobmsg_get_u32(tb[ATTR_TECH]) : 0;
	_status.acchg_online = tb[ATTR_AC_ONLINE] ?
		blobmsg_get_u32(tb[ATTR_AC_ONLINE]) : 0;
	_status.acchg_type = tb[ATTR_AC_TYPE] ?
		blobmsg_get_u32(tb[ATTR_AC_TYPE]) : 0;
	_status.acchg_status = tb[ATTR_AC_STATUS] ?
		blobmsg_get_u32(tb[ATTR_AC_STATUS]) : 0;
	_status.usbchg_online = tb[ATTR_USB_ONLINE] ?
		blobmsg_get_u32(tb[ATTR_USB_ONLINE]) : 0;
	_status.usbchg_type = tb[ATTR_USB_TYPE] ?
		blobmsg_get_u32(tb[ATTR_USB_TYPE]) : 0;
	_status.usbchg_status = tb[ATTR_USB_STATUS] ?
		blobmsg_get_u32(tb[ATTR_USB_STATUS]) : 0;

	return 0;
}

void MGuiCharger::dump_chg_bat_tag()
{
	ILOG_DEBUG(MGUI_CHARGER, "\nBattery capacity: %d/100\n",_status.capacity);
	ILOG_DEBUG(MGUI_CHARGER, "    Battery type: %d\n",_status.type);
	ILOG_DEBUG(MGUI_CHARGER, " Battery present: %d\n",_status.present);
	ILOG_DEBUG(MGUI_CHARGER, "  Battery health: %d\n",_status.health);
	ILOG_DEBUG(MGUI_CHARGER, " Battery voltage: %d\n",_status.voltage_now);
	ILOG_DEBUG(MGUI_CHARGER, "    Battery temp: %d\n",_status.bat_temp);
	ILOG_DEBUG(MGUI_CHARGER, "  Battery status: %d\n",_status.bat_status);
	ILOG_DEBUG(MGUI_CHARGER, "    Battery tech: %d\n",_status.bat_technology);
	ILOG_DEBUG(MGUI_CHARGER, "    Acchg online: %d\n",_status.acchg_online);
	ILOG_DEBUG(MGUI_CHARGER, "      Acchg type: %d\n",_status.acchg_type);
	ILOG_DEBUG(MGUI_CHARGER, "    Acchg status: %d\n",_status.acchg_status);
	ILOG_DEBUG(MGUI_CHARGER, "   USBchg online: %d\n",_status.usbchg_online);
	ILOG_DEBUG(MGUI_CHARGER, "     USBchg type: %d\n",_status.usbchg_type);
	ILOG_DEBUG(MGUI_CHARGER, "   USBchg status: %d\n",_status.usbchg_status);	
}

int MGuiCharger::Update(blob_attr *msg)
{
	int ret = parseattrs_tag(msg);
	if (ret){
		ILOG_ERROR(MGUI_CHARGER, "parsing blobmsg failed %d\n", ret);
		return ret;
	}

	dump_chg_bat_tag();

	if (_status.bat_status == STATUS_UNKNOWN)
		_bar->setBatteryState(BatteryUnknown);
	else if (_status.bat_status == STATUS_CHARGING) {
		if (_status.capacity == 100)
			_bar->setBatteryState(BatteryCharging100);
		else if (_status.capacity >= 90)
			_bar->setBatteryState(BatteryCharging90);
		else if (_status.capacity >= 80)
			_bar->setBatteryState(BatteryCharging80);
		else if (_status.capacity >= 60)
			_bar->setBatteryState(BatteryCharging60);
		else if (_status.capacity >= 50)
			_bar->setBatteryState(BatteryCharging50);
		else if (_status.capacity >= 30)
			_bar->setBatteryState(BatteryCharging30);
		else if (_status.capacity >= 20)
			_bar->setBatteryState(BatteryCharging20);
		else
			_bar->setBatteryState(BatteryCharging20);
	} else {
		if (_status.capacity == 100)
			_bar->setBatteryState(Battery100);
		else if (_status.capacity >= 90)
			_bar->setBatteryState(Battery90);
		else if (_status.capacity >= 80)
			_bar->setBatteryState(Battery80);
		else if (_status.capacity >= 60)
			_bar->setBatteryState(Battery60);
		else if (_status.capacity >= 50)
			_bar->setBatteryState(Battery50);
		else if (_status.capacity >= 30)
			_bar->setBatteryState(Battery30);
		else if (_status.capacity >= 20)
			_bar->setBatteryState(Battery20);
		else
			_bar->setBatteryState(BatteryAlert);
	}

	return 0;
}

int MGuiCharger::Indication(struct ubus_context *ctx, struct ubus_object *obj,
			      struct ubus_request_data *req,
			      const char *method, struct blob_attr *msg)
{
	return Update(msg);
}

MGuiCharger* MGuiCharger::Instance()
{
	return _instance;
}

int MGuiCharger::Create(ubus_context *ubus, StatusBar *bar)
{
	int ret;

	ILOG_TRACE_F(MGUI_CHARGER);
	if (_instance) {
		ILOG_ERROR(MGUI_CHARGER, "Already created\n");
		return -1;
	}

	_instance = new MGuiCharger(ubus, bar);
	ret = _instance->Register();
	if (ret) {
		ILOG_ERROR(MGUI_CHARGER, "Register failed (ret=%d)\n", ret);
		goto out_error;
	}
	_instance->Register();

	ILOG_DEBUG(MGUI_CHARGER, "%s exit\n", __FUNCTION__);
	return 0;

out_error:
	delete _instance;
	_instance = NULL;
}


void MGuiCharger::Destroy()
{
	ILOG_TRACE_F(MGUI_CHARGER);

	if (_instance) {
		_instance->UnRegister();
		_instance = NULL;
	}
}

MGuiCharger::MGuiCharger(ubus_context *ubus, StatusBar *bar)
 : UBusClient(ubus),
   _bar(bar)
{
}

MGuiCharger::~MGuiCharger()
{
}

int MGuiCharger::Register()
{
	int ret;

	ILOG_TRACE(MGUI_CHARGER);

	ret = UBusClient::Register(charger_indication, CHARGER_UBUS_ID);
	if (ret) {
		ILOG_ERROR(MGUI_CHARGER, "Register failed %d\n", ret);
		return ret;
	}

	_request = new ChargerRequest(_subscriber, _id, _ubus);
	_request->Request();

	ILOG_DEBUG(MGUI_CHARGER, "%s exit\n", __FUNCTION__);
	return 0;

unregister:
	UBusClient::UnRegister();
	return ret;
}

int MGuiCharger::UnRegister()
{
	return UBusClient::UnRegister();
}

};
