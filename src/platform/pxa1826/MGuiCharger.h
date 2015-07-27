#ifndef SRC_MGUI_CHARGER_H_
#define SRC_MGUI_CHARGER_H_

#include "UBusClient.h"
#include "ChargerRequest.h"

namespace MGUI
{

class ChargerRequest;

class MGuiCharger : public UBusClient
{

public:
	static MGuiCharger*
	Instance();

	int
	Indication(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req,
		      const char *method, struct blob_attr *msg);

	static int
	Create(ubus_context *ubus, StatusBar *bar);

	static void
	Destroy();

	virtual int
	Register();

	virtual int
	UnRegister();

	int
	Update(blob_attr *msg);

protected:
	MGuiCharger(ubus_context* ubus, StatusBar* bar);

	virtual
	~MGuiCharger();

private:
	enum chg_status_stat {
		STATUS_UNKNOWN = 0,
		STATUS_CHARGING,
		STATUS_DISCHARGING,
		STATUS_NOT_CHARGING,
		STATUS_FULL,
	};

	enum {
		ATTR_CAPACITY = 0,
		ATTR_TYPE,
		ATTR_PRESENT,
		ATTR_HEALTH,
		ATTR_VOLTAGE,
		ATTR_TEMP,
		ATTR_STATUS,
		ATTR_TECH,
		ATTR_AC_ONLINE,
		ATTR_AC_TYPE,
		ATTR_AC_STATUS,
		ATTR_USB_ONLINE,
		ATTR_USB_TYPE,
		ATTR_USB_STATUS,
		NR_ATTR,
	};

	struct chg_bat_tag {
		unsigned int capacity;
		unsigned int health;
		unsigned int present;
		unsigned int type;
		unsigned int voltage_now;
		unsigned int bat_status;
		unsigned int bat_temp;
		unsigned int bat_technology;
		unsigned int acchg_online;
		unsigned int acchg_type;
		unsigned int acchg_status;
		unsigned int usbchg_online;
		unsigned int usbchg_type;
		unsigned int usbchg_status;
	} _status;

	int
	parse_blobmsg(blob_attr *attr, blob_attr **tb);

	int
	charger_parseattrs_tag(blob_attr *msg);

	void
	dump_chg_bat_tag();

	static MGuiCharger* _instance;
	ChargerRequest* _request;
	StatusBar* _bar;
};

} /* namespace MGUI */

#endif /* SRC_MGUI_CHARGER_H_ */
