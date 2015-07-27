#ifndef SRC_MGUI_HAWK_REQUEST_H_
#define SRC_MGUI_HAWK_REQUEST_H_

#include <MGuiPlatform.h>
#include "UBusClient.h"
#include "UBusRequest.h"
#include "MGuiHawk.h"

namespace MGUI
{

class HawkRequest : public UBusRequest
{
public:

	HawkRequest(ubus_subscriber &subscriber,
		    uint32_t &ubus_id, ubus_context *ubus);

	virtual
	~HawkRequest();

	int
	Request(enum PlatformRequest &req);

	virtual int
	Callback(ubus_request *req, blob_attr *msg);

private:
	const char *
	req_to_str(enum PlatformRequest &req);
};

};

#endif /* SRC_MGUI_HAWK_REQUEST_H_ */
