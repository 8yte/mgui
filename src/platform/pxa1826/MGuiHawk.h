#ifndef SRC_MGUI_HAWK_H_
#define SRC_MGUI_HAWK_H_

#include "UBusClient.h"
#include "HawkRequest.h"

namespace MGUI
{

class HawkRequest;

class MGuiHawk : public UBusClient
{

public:
	static MGuiHawk*
	Instance();

	int
	Indication(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req,
		      const char *method, struct blob_attr *msg);

	static int
	Create(ubus_context *ubus, BottomBar *bar);

	static void
	Destroy();

	virtual int
	Register();

	virtual int
	UnRegister();

	void
	Reset();

	void
	Fota();

	void
	Assert();

	void
	KeepAlive();

	void
	ForceUpload();

protected:
	MGuiHawk(ubus_context* ubus, BottomBar *bar);

	virtual
	~MGuiHawk();

private:
	std::string
	GetVersion();

	void
	SetVersion();

	static MGuiHawk* _instance;
	HawkRequest* _request;
	BottomBar* _bar;
};

} /* namespace MGUI */

#endif /* SRC_MGUI_HAWK_H_ */
