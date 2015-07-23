#include <core/Logger.h>
#include <ui/StatusBar.h>
#include "RilRequest.h"

#define RIL_UBUS_REQ		"ril_request"

namespace MGUI
{

D_DEBUG_DOMAIN(MGUI_RILREQ, "Mgui/RilRequest", "RilRequest");
D_DEBUG_DOMAIN(MGUI_OPERATOR, "Mgui/RilRequest/Operator", "OperatorRequest");
D_DEBUG_DOMAIN(MGUI_SIMCARD, "Mgui/RilRequest/Simcard", "SimcardRequest");
D_DEBUG_DOMAIN(MGUI_SCREEN, "Mgui/RilRequest/Screen", "ScreenRequest");
D_DEBUG_DOMAIN(MGUI_REGISTRATION, "Mgui/RilRequest/Registration", "RegistrationRequest");
D_DEBUG_DOMAIN(MGUI_INDICATION, "Mgui/RilIndication", "RilIndication");

/***************************************************************************/
/**		RilRequest Class Implementation
/***************************************************************************/
RilRequest::RilRequest(ubus_subscriber &subscriber, uint32_t &ubus_id, ubus_context *ubus)
 : UBusRequest(subscriber, ubus_id, ubus)
{
}

RilRequest::~RilRequest()
{
}

int RilRequest::Request(uint32_t id, const void *data, int len)
{
	int ret = 0;

	ILOG_TRACE(MGUI_RILREQ);
	ret = rilutil_makeRequestBlob(&_b, id, data, len);
	if (ret) {
		ILOG_ERROR(MGUI_RILREQ, "rilutil_makeRequestBlob failed (ret=%d)\n", ret);
		return ret;
	}
	ret = InvokeAsync(RIL_UBUS_REQ, _b.head);
	if (ret) {
		ILOG_ERROR(MGUI_RILREQ, "Invoke() failed (ret=%d)\n", ret);
		return ret;
	}
	ILOG_DEBUG(MGUI_RILREQ, "%s exit\n", __FUNCTION__);

	return ret;
}

int RilRequest::Request(uint32_t id)
{
	return Request(id, 0, 0);
}

void RilRequest::Indication(uint32_t &id, void *data, int len)
{
}

/***************************************************************************/
/**		RilOperator Class Implementation
/***************************************************************************/
RilOperator::RilOperator(ubus_subscriber &subscriber, uint32_t &ubus_id, ubus_context* ubus, StatusBar *bar)
	: RilRequest(subscriber, ubus_id, ubus)
{
	_bar = bar;
}

RilOperator::~RilOperator()
{
}

int RilOperator::Request()
{
	ILOG_DEBUG(MGUI_OPERATOR, "Request operator\n");
	return RilRequest::Request(RIL_REQUEST_OPERATOR);
}

int RilOperator::Callback(ubus_request *req, blob_attr *msg)
{
	unsigned int requestid;
	unsigned int rilerrno = 0;
	void *data = NULL;
	int datalen = 0;
	int ret = 0;

	ILOG_TRACE(MGUI_OPERATOR);

	if(!req || !msg) {
		ILOG_ERROR(MGUI_OPERATOR, "request or message is NULL! (req=%p, msg=%p)", req, msg);
		return -1;
	}

	ret = rilutil_parseResponse(msg, &requestid, &rilerrno, &data, &datalen);
	rilutilstrings *resp = (rilutilstrings *)data;

	if (ret || rilerrno) {
		ILOG_ERROR(MGUI_OPERATOR, "RIL ERROR: ret=%d, requestid=%d, rilerrno=%d\n",
			  ret, requestid, rilerrno);
		return ret;
	}

	if (!resp || resp->num != 3) {
		ILOG_ERROR(MGUI_OPERATOR, "No operator found\n");
		//TODO: UPDATE OPERATOR TO NO SERVICE
	} else {
		/* operator present */
		ILOG_DEBUG(MGUI_OPERATOR, "Operator = %s\n", resp->str[0]);
		//TODO: UPDATE OPERATOR TO resp->str[0]
	}

	if (data)
		free(data);

	ILOG_DEBUG(MGUI_OPERATOR, "%s:%d: Exit\n", __FUNCTION__, __LINE__);
	return 0;
}

/***************************************************************************/
/**		RilSimcard Class Implementation
/***************************************************************************/
RilSimcard::RilSimcard(ubus_subscriber &subscriber, uint32_t &ubus_id, ubus_context* ubus, StatusBar *bar)
	: RilRequest(subscriber, ubus_id, ubus)
{
	_bar = bar;
}

RilSimcard::~RilSimcard()
{
}

int RilSimcard::Request()
{
	ILOG_DEBUG(MGUI_SIMCARD, "Request simcard status\n");
	return RilRequest::Request(RIL_REQUEST_GET_SIM_STATUS);
}

int RilSimcard::Callback(ubus_request *req, blob_attr *msg)
{
	unsigned int requestid;
	unsigned int rilerrno = 0;
	void *data = NULL;
	int datalen = 0;
	int ret = 0;

	ILOG_TRACE(MGUI_SIMCARD);

	if(!req || !msg) {
		ILOG_ERROR(MGUI_SIMCARD, "request or message is NULL! (req=%p, msg=%p)", req, msg);
		return -1;
	}

	ret = rilutil_parseResponse(msg, &requestid, &rilerrno, &data, &datalen);
	if (ret || rilerrno || !data) {
		ILOG_ERROR(MGUI_SIMCARD, "RIL ERROR: ret=%d, requestid=%d, rilerrno=%d, data=%p\n",
			  ret, requestid, rilerrno, data);
		return ret;
	}

	RIL_CardState SimStatus = ((RIL_CardStatus_v6 *)data)->card_state;

	switch (SimStatus) {
	case RIL_CARDSTATE_ABSENT:
		_bar->setSimState(SimMissing);
		ILOG_DEBUG(MGUI_SIMCARD, "Simcard absent\n");
		break;
	case RIL_CARDSTATE_PRESENT:
		_bar->setSimState(SimActive);
		ILOG_DEBUG(MGUI_SIMCARD, "Simcard present\n");
		break;
	case RIL_CARDSTATE_ERROR:
		_bar->setSimState(SimUnknown);
		ILOG_DEBUG(MGUI_SIMCARD, "Simcard error\n");
		break;
	}

	free(data);

	ILOG_DEBUG(MGUI_SIMCARD, "%s:%d: Exit\n", __FUNCTION__, __LINE__);
	return 0;
}

/***************************************************************************/
/**		RilScreen Class Implementation
/***************************************************************************/

RilScreen::RilScreen(ubus_subscriber &subscriber, uint32_t &ubus_id, ubus_context* ubus)
	: RilRequest(subscriber, ubus_id, ubus)
{
}

RilScreen::~RilScreen()
{
}

int RilScreen::Request(int on_off)
{
	ILOG_DEBUG(MGUI_SCREEN, "Request screen %s\n",on_off ? "on" : "off");
	return RilRequest::Request(RIL_REQUEST_SCREEN_STATE, (const void *)&on_off, (int)1);
}

int RilScreen::Callback(ubus_request *req, blob_attr *msg)
{
	return 0;
}

/***************************************************************************/
/**		RilRegistration Class Implementation
/***************************************************************************/
RilRegistration::RilRegistration(ubus_subscriber &subscriber, uint32_t &ubus_id, ubus_context* ubus, StatusBar *bar)
	: RilRequest(subscriber, ubus_id, ubus)
{
	_bar = bar;
}

RilRegistration::~RilRegistration()
{
}

bool RilRegistration::registered_voice()
{
	return (_voice.reg_state == 1 || _voice.reg_state == 5);
}

bool RilRegistration::registered_data()
{
	return (_data.reg_state == 1 || _data.reg_state == 5);
}

CellularTechState
RilRegistration::tech()
{
	switch (_voice.radio_tech) {
	case RADIO_TECH_GPRS:	return CellularTech2G; /* 2G */
	case RADIO_TECH_EDGE:	return CellularTechEdge; /* E */
	case RADIO_TECH_UMTS:	return CellularTech3G; /* 3G */
	case RADIO_TECH_HSDPA:
	case RADIO_TECH_HSUPA:
	case RADIO_TECH_HSPAP:	return CellularTechHSDPAP; /* H */
	case RADIO_TECH_HSPA:	return CellularTechHSDPA; /* H+ */
	case RADIO_TECH_LTE:	return CellularTech4G; /* 4G */
	case RADIO_TECH_GSM:	return CellularTechG; /* G */
	default:		return CellularTechUnknown; /* ? */
	}
}

const char *
RilRegistration::tech_to_str()
{
	
	switch (_voice.radio_tech) {
	case RADIO_TECH_GPRS:	return "2G";
	case RADIO_TECH_EDGE:	return "EDGE";
	case RADIO_TECH_UMTS:	return "3G";
	case RADIO_TECH_HSDPA:
	case RADIO_TECH_HSUPA:
	case RADIO_TECH_HSPAP:	return "H+";
	case RADIO_TECH_HSPA:	return "H";
	case RADIO_TECH_LTE:	return "4G";
	case RADIO_TECH_GSM:	return "GSM";
	default:		return "Unknown";
	}
}

int RilRegistration::convertDbmToRssi(int rsrp)
{
	if(rsrp == INT_MAX)
		_rssi = 99;
	else if(rsrp < -113)
		_rssi = 0;
	else if(rsrp > -51)
		_rssi =31;
	else
		_rssi = (113 + rsrp) / 2;
}

void RilRegistration::Indication(uint32_t &id, void *data, int len)
{
	ILOG_TRACE(MGUI_REGISTRATION);

	switch (id) {
	case RIL_UNSOL_SIGNAL_STRENGTH:
		RIL_SignalStrength_v6 *rssi = (RIL_SignalStrength_v6 *)data;
		if (tech() == CellularTech4G) {
			ILOG_DEBUG(MGUI_INDICATION, "in LTE, using rsrp == %d as rssi value\n",
				  rssi->LTE_SignalStrength.rsrp);
			convertDbmToRssi(rssi->LTE_SignalStrength.rsrp);
		} else {
			ILOG_DEBUG(MGUI_INDICATION, "Not LTE, rssi = %d\n",
				   rssi->GW_SignalStrength.signalStrength);
			_rssi = rssi->GW_SignalStrength.signalStrength;
		}
		ILOG_DEBUG(MGUI_REGISTRATION, "rssi value changed to %d\n", _rssi);
		if (!registered_voice()) {
			ILOG_DEBUG(MGUI_REGISTRATION, "no voice, no data\n");
			_bar->setCellularState(CellularNull);
		} else if (!registered_data()) {
			ILOG_DEBUG(MGUI_INDICATION, "voice, no data\n");
			if (_rssi >= 18)
				_bar->setCellularState(Cellular4NoData);
			else if (_rssi >= 13)
				_bar->setCellularState(Cellular3NoData);
			else if (_rssi >= 10)
				_bar->setCellularState(Cellular2NoData);
			else if (_rssi >= 5)
				_bar->setCellularState(Cellular1NoData);
			else
				_bar->setCellularState(Cellular0NoData);
		} else {
			ILOG_DEBUG(MGUI_INDICATION, "voice and data\n");
			if (_rssi >= 18)
				_bar->setCellularState(Cellular4);
			else if (_rssi >= 13)
				_bar->setCellularState(Cellular3);
			else if (_rssi >= 10)
				_bar->setCellularState(Cellular2);
			else if (_rssi >= 5)
				_bar->setCellularState(Cellular1);
			else
				_bar->setCellularState(Cellular0);
		}
		break;
	}
	ILOG_DEBUG(MGUI_INDICATION, "%s:%d: Exit\n", __FUNCTION__, __LINE__);
}

int RilRegistration::Request()
{
	int ret;

	ILOG_TRACE(MGUI_REGISTRATION);

	ILOG_DEBUG(MGUI_REGISTRATION, "Request voice registration\n");
	ret = RilRequest::Request(RIL_REQUEST_VOICE_REGISTRATION_STATE);
	if (ret) {
		ILOG_ERROR(MGUI_RILREQ, "request voice registration failed (ret=%d)\n", ret);
		return ret;
	}

	ILOG_DEBUG(MGUI_REGISTRATION, "Request data registration\n");
	ret = RilRequest::Request(RIL_REQUEST_DATA_REGISTRATION_STATE);
	if (ret) {
		ILOG_ERROR(MGUI_REGISTRATION, "request voice registration failed (ret=%d)\n", ret);
		return ret;
	}

	ILOG_DEBUG(MGUI_REGISTRATION, "%s:%d: Exit\n", __FUNCTION__, __LINE__);

	return 0;
}

int RilRegistration::Callback(ubus_request *req, blob_attr *msg)
{
	unsigned int requestid;
	unsigned int rilerrno = 0;
	void *data = NULL;
	int datalen = 0;
	int ret = 0;
	RegState *result;

	ILOG_TRACE(MGUI_REGISTRATION);
	if(!req || !msg) {
		ILOG_ERROR(MGUI_REGISTRATION, "request or msg is NULL (req=%p, msg=%p)\n", req, msg);
		return -1;
	}

	ret = rilutil_parseResponse(msg, &requestid, &rilerrno, &data, &datalen);
	rilutilstrings *resp = (rilutilstrings *)data;

	if (resp->num < 2) {
		ILOG_ERROR(MGUI_REGISTRATION, "wrong resp->num=%d\n", resp->num);
		return -1;
	}

	result = (requestid == RIL_REQUEST_VOICE_REGISTRATION_STATE) ? &_voice : &_data;

	sscanf(resp->str[0], "%d", &result->reg_state);
	if (resp->num >= 5)
		sscanf(resp->str[3], "%d", (int *)&result->radio_tech);

	if (data)
		free(data);

	if (requestid == RIL_REQUEST_VOICE_REGISTRATION_STATE) {
		ILOG_DEBUG(MGUI_REGISTRATION, "Voice: %s registered\n", registered_voice() ? "" : "not");
		_bar->setCellularState(Cellular0NoData);
	}

	if (requestid == RIL_REQUEST_DATA_REGISTRATION_STATE) {
		ILOG_DEBUG(MGUI_REGISTRATION, "Data: %s registered\n", registered_data() ? "" : "not");
		_bar->setCellularState(Cellular0);
	}

	_bar->setCellularTechState(tech());

	ILOG_DEBUG(MGUI_REGISTRATION, "Radio Technology: %s\n", tech_to_str());
	ILOG_DEBUG(MGUI_REGISTRATION, "%s:%d: Exit\n", __FUNCTION__, __LINE__);

	return 0;
}

};
