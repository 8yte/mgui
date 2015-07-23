#include <core/Logger.h>
#include "UBusRequest.h"

namespace MGUI
{

D_DEBUG_DOMAIN(UBUS_REQ, "Ubus/Request", "Request");

static void data_cb(struct ubus_request *req, int type, struct blob_attr *msg)
{
	UBusRequest *__this = (UBusRequest *)req->priv;
	__this->Callback(req, msg);
}

static void complete_cb(struct ubus_request *req, int ret)
{
	free(req);
}

UBusRequest::UBusRequest(ubus_subscriber &subscriber, uint32_t &id, ubus_context* ubus)
{

	ILOG_TRACE(UBUS_REQ);
	_subscriber = subscriber;
	_id = id;
	_ubus = ubus;
	ILOG_DEBUG(UBUS_REQ, "%s exit\n", __func__);
}

UBusRequest::~UBusRequest()
{
	ILOG_TRACE(UBUS_REQ);
	ILOG_DEBUG(UBUS_REQ, "%s exit\n", __func__);
}

int UBusRequest::Callback(ubus_request *req, blob_attr *msg)
{
	return 0;
}

int UBusRequest::Invoke(const char *method, blob_attr *b)
{
	int ret = 0;

	ILOG_TRACE(UBUS_REQ);

	ILOG_DEBUG(UBUS_REQ, "%s enter\n", __func__);
	ret = ubus_invoke(_ubus, _id, method, b , data_cb, this, 0);
	if (ret) {
		ILOG_ERROR(UBUS_REQ,"ubus_invoke failed [id=%d, method=%s, err=%s]\n",
			  _id, method, ubus_strerror(ret));
		return ret;
	}
	
	ILOG_DEBUG(UBUS_REQ,"ubus_invoke done [id=%d, method=%s]\n",
		  _id, method, ubus_strerror(ret));

	ILOG_DEBUG(UBUS_REQ, "%s exit\n", __func__);
	return 0;
}

int UBusRequest::InvokeAsync(const char *method, blob_attr *b)
{
	ubus_request *req;
	int ret = 0;

	ILOG_TRACE(UBUS_REQ);
	ILOG_DEBUG(UBUS_REQ, "%s enter\n", __func__);

	req = (ubus_request *)malloc(sizeof(*req));
	if (!req) {
		ILOG_ERROR(UBUS_REQ,"memory allocation failed\n");
		return -1;
	}

	ret = ubus_invoke_async(_ubus, _id, method, b, req);
	if (ret) {
		ILOG_ERROR(UBUS_REQ, "ubus_invoke failed [id=%d, method=%s, err=%s]\n",
			  _id, method, ubus_strerror(ret));
		free(req);
		return ret;
	}
	ILOG_DEBUG(UBUS_REQ,"ubus_invoke_async done [id=%d, method=%s]\n", _id, method);

	req->data_cb = data_cb;
	req->complete_cb = complete_cb;
	req->priv = this;
	ubus_complete_request_async(_ubus, req);

	ILOG_DEBUG(UBUS_REQ, "ubus_complete_request_async done\n");

	return 0;
}

}; /* namespace MGUI */
