#include <core/Logger.h>
#include <ui/BottomBar.h>
#include <cutils/properties.h>
#include <iostream>
#include <fstream>
#include "MGuiHawk.h"

#define HAWK_UBUS_ID	"hawk"

namespace MGUI
{

D_DEBUG_DOMAIN(MGUI_HAWK, "Mgui/Hawk", "Hawk");

MGuiHawk *MGuiHawk:: _instance = 0;

static int hawk_indication(struct ubus_context *ctx, struct ubus_object *obj,
			      struct ubus_request_data *req,
			      const char *method, struct blob_attr *msg)
{
	MGuiHawk *__this = MGuiHawk::Instance();
	if (!__this) {
		ILOG_ERROR(MGUI_HAWK, "No instance, indication ignored!\n");
		return -1;
	}
		
	return __this->Indication(ctx, obj, req, method, msg);
}

int MGuiHawk::Indication(struct ubus_context *ctx, struct ubus_object *obj,
			      struct ubus_request_data *req,
			      const char *method, struct blob_attr *msg)
{
	return 0;
}

MGuiHawk* MGuiHawk::Instance()
{
	return _instance;
}

#define VERSION_PATH "/etc/mversion"

std::string
MGuiHawk::GetVersion()
{
	std::string line;
	std::ifstream mversion("/etc/mversion");

	if (mversion.is_open()) {
		getline(mversion, line);
		mversion.close();
	} else {
		ILOG_ERROR(MGUI_HAWK, "can't open /etc/mversion\n");
		line.assign("ERROR");
	}

	return line;	
}

void MGuiHawk::SetVersion()
{
	int len, retries = 20;
	char hawk_version[100];
	std::string version("Hawk Version: ");

	len = property_get("persist.hawk.build_version", hawk_version, "Version Unknown");

	while (!len && --retries) {
		ILOG_INFO(MGUI_HAWK, "retry get hawk version");
		len = property_get("persist.hawk.build_version", hawk_version, "Version Unknown");
		sleep(1);
	}

	if (len) {
		ILOG_INFO(MGUI_HAWK, "hawk version: %s", hawk_version);
	} else {
		ILOG_INFO(MGUI_HAWK, "Failed to get hawk version");
	}

	version = version + std::string(hawk_version);

	_bar->SetVersion(version);
}

int MGuiHawk::Create(ubus_context *ubus, BottomBar *bar)
{
	int ret;

	ILOG_TRACE_F(MGUI_HAWK);
	if (_instance) {
		ILOG_ERROR(MGUI_HAWK, "Already created\n");
		return -1;
	}

	_instance = new MGuiHawk(ubus, bar);

	_instance->SetVersion();

	ret = _instance->Register();
	if (ret) {
		ILOG_ERROR(MGUI_HAWK, "Register failed (ret=%d)\n", ret);
		goto out_error;
	}
	_instance->Register();

	ILOG_DEBUG(MGUI_HAWK, "%s exit\n", __FUNCTION__);
	return 0;

out_error:
	delete _instance;
	_instance = NULL;
}


void MGuiHawk::Destroy()
{
	ILOG_TRACE_F(MGUI_HAWK);

	if (_instance) {
		_instance->UnRegister();
		_instance = NULL;
	}
}

MGuiHawk::MGuiHawk(ubus_context *ubus, BottomBar *bar)
 : UBusClient(ubus),
   _bar(bar)
{
}

MGuiHawk::~MGuiHawk()
{
}

void MGuiHawk::Reset()
{
	if (_instance)
		_request->Request(HawkResetReq);
}

void MGuiHawk::Fota()
{
	if (_instance)
		_request->Request(HawkFotaReq);
}

void MGuiHawk::Assert()
{
	if (_instance)
		_request->Request(HawkNoDataAssertReq);
}

void MGuiHawk::KeepAlive()
{
	if (_instance)
		_request->Request(HawkKeepAliveReq);
}

void MGuiHawk::ForceUpload()
{
	if (_instance)
		_request->Request(HawkForceUploadReq);
}

int MGuiHawk::Register()
{
	int ret;

	ILOG_TRACE(MGUI_HAWK);

	ret = UBusClient::Register(hawk_indication, HAWK_UBUS_ID);
	if (ret) {
		ILOG_ERROR(MGUI_HAWK, "Register failed %d\n", ret);
		return ret;
	}

	ret = UBusClient::Subscribe();
	if (ret) {
		ILOG_ERROR(MGUI_HAWK, "Subscribe events failed %d\n", ret);
		goto unregister;
	}

	_request = new HawkRequest(_subscriber, _id, _ubus);

	ILOG_DEBUG(MGUI_HAWK, "%s exit\n", __FUNCTION__);
	return 0;

unregister:
	UBusClient::UnRegister();
	return ret;
}

int MGuiHawk::UnRegister()
{
	delete _request;
	return UBusClient::UnRegister();
}

};
