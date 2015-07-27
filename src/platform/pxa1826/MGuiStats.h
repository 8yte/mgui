#ifndef SRC_MGUI_STATS_H_
#define SRC_MGUI_STATS_H_

#include "UBusClient.h"
#include "StatsRequest.h"

namespace MGUI
{

class StatsRequest;

class MGuiStats : public UBusClient
{

public:
	static MGuiStats*
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
	MGuiStats(ubus_context* ubus, StatusBar* bar);

	virtual
	~MGuiStats();

private:
	enum {
		ATTR_STATISTICS,
		ATTR_WIFI_STATUS,
		NR_ATTR,
	};
	static const blobmsg_policy _policy[NR_ATTR];

	int
	parseattrs_req(blob_attr *msg);

	enum {
		REQID,
		DATA,
		_MAX
	};

	enum {
		WIFI_CLIENTS_NOTIFY_ID,
		WIFI_STATUS_NOTIFY_ID,
	};

	int
	parseattrs_ind(struct blob_attr *attr, struct blob_attr **tb);

	static MGuiStats* _instance;
	StatsRequest* _request;
	StatusBar* _bar;
	struct stats_info {
		std::string wifi_status;
		int num_clients;
	} _status;

};

} /* namespace MGUI */

#endif /* SRC_MGUI_STATS_H_ */
